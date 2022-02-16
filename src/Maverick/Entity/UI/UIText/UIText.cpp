#include "UIText.hpp"

#include "../../../Engine/Engine.hpp"
extern Engine* engine;

/** UIText constructor (for viewport calcs, 1.0 is one screen width/height)
 * @param name The name of the UIElement
 * @param view_x_offset The viewport X offset of the UIElement
 * @param view_y_offset The viewport X offset of the UIElement
 * @param view_width The viewport width of the UIElement
 * @param view_height The viewport height of the UIElement
 * @param animation_num The animation number of the UIElement (use for multiple would be blinking cursors)
 * @param draw_layer The draw layer of the UIElement (all child elements will be drawn directly on top)
 * @param window The current window (used for viewport calculation)
 * @param text The text to display
 * @param font_path The filepath to the .ttf file you wish to use
 * @param scroll_speed The speed at which text scrolls (in units of chars/sec)
 * @param point The point (size) of the text
 * @param x_alignment The horizontal alignment of the text
 * @param y_alignment The vertical alignment of the text
 */
UIText::UIText(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
               SDL_Window* window, int draw_layer, const char* font_path, const char* text,
               float scroll_speed, unsigned int point, ALIGNMENT x_alignment, ALIGNMENT y_alignment)
    : UIElement(name, view_x_offset, view_y_offset, view_width, view_height, window, draw_layer){
    this->type = UI_ELEMENT_TYPE::TEXT;

    //Setting the text's alignment
    this->x_alignment = x_alignment;
    this->y_alignment = y_alignment;

    this->view_height = view_height;
    this->view_width = view_width;

    this->point = point;
    this->scroll_speed = scroll_speed;

    this->num_lines = 0;
    this->chars_per_line = 0;

    this->print_buff = nullptr;
    this->ref_buff = nullptr;
    this->text = nullptr;
    this->font = nullptr;

    this->setFont(font_path);
    this->setPoint(point);

    //Default color is white
    this->font_color = SDL_Color{0xFF, 0xFF, 0xFF, 0xFF};

    this->setText(text);
}

/** UIText Destructor
 */
UIText::~UIText(){
    if(this->font != nullptr){
        TTF_CloseFont(this->font);
    }
    if(this->text != nullptr){
        free(this->text);
    }
    if(this->print_buff != nullptr){
        free(this->print_buff);
    }
}

/** Sets the text's contents 
 * @param text The text you wish to use (will make a local copy of the text)
 */
void UIText::setText(const char* text){
    //Reset the text timer (since we have new text)
    this->timer = 0.0;

    unsigned int len = strlen(text);

    //Allocate & copy the text
    if(this->text != nullptr){
        free(this->text);
    }
    this->text = (char*)malloc(len + 1);

    for(size_t i = 0; i < len; i++){
        this->text[i] = text[i];
    }
    this->text[len] = '\0';

    //Set buffer cursor to the start of the text
    this->text_buff_cursor = this->text;

    this->nextPage();
}

/** Sets the text's font (call upon change of font or point)
 * @param font_path The path to a .ttf you wish to use
 */
void UIText::setFont(const char* font_path){
    if(font_path != nullptr){
        if(this->font != nullptr){
            TTF_CloseFont(this->font);
        }

        this->font_path = font_path;
        this->font = TTF_OpenFont(font_path, point);
        if(font == nullptr){
            fprintf(stderr, "Could not open texture: %s\n", TTF_GetError());
            return;
        }

        this->setStyle(TTF_STYLE_NORMAL);
    }
    else{
        fprintf(stderr, "Failed to set font; font_path is null!\n");
    }
}

/** Sets the point (size) of the font
 * @param point The point (size) of the font
 */
void UIText::setPoint(unsigned int point){
    if(this->font_path != nullptr){
        this->point = point;
        this->setFont(this->font_path);

        int char_width = 0;
        int char_height = 0;

        //Set line size and num of 
        if(TTF_SizeUTF8(this->font, "W", &char_width, &char_height) == 0){
            if(this->window != nullptr){
                //Free the old buffers
                if(this->print_buff != nullptr){
                    for(unsigned int i = 0; i < this->num_lines; i++){
                        free(this->print_buff[i]);
                        free(this->ref_buff[i]);
                    }
                    free(this->print_buff);
                    free(this->ref_buff);
                }

                //Calculate the print_buffer attributes
                this->num_lines = this->view_height * SCREEN_WIDTH / char_height;
                this->chars_per_line = this->view_width * SCREEN_HEIGHT / char_width;

                //Allocate space for the new buffers
                this->print_buff = (char**)malloc(sizeof(char*) * num_lines);
                this->ref_buff = (char**)malloc(sizeof(char*) * num_lines);
                for(unsigned int i = 0; i < num_lines; i++){
                    this->print_buff[i] = (char*)calloc(1, chars_per_line + 1);
                    this->ref_buff[i] = (char*)calloc(1, chars_per_line + 1);
                }
            }
        }
    }
}

/** Sets the style of the font
 * @param style Any number of TTF_STYLE_* attrs OR'd together
 */
void UIText::setStyle(uint8_t style){
    if(this->font != nullptr){
        TTF_SetFontStyle(this->font, style);
    }
}

/** Sets the color of font
 * @param r The red value of the font
 * @param g The green value of the font
 * @param b The blue value of the font
 * @param a The alpha value of the font
 */
void UIText::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    this->font_color = SDL_Color{r, g, b, a};
}

/** Sets the text scroll speed
 * @param scroll_speed The speed of text scroll speed (in chars/sec)
 */
void UIText::setScrollSpeed(float scroll_speed){
    this->scroll_speed = scroll_speed;
}


/** Sets the X alignment
 * @param x_alignment The X alignment
 */
void UIText::setXAlignment(ALIGNMENT x_alignment){
    this->x_alignment = x_alignment;
}

/** Sets the Y alignment
 * @param y_alignment The Y alignment
 */
void UIText::setYAlignment(ALIGNMENT y_alignment){
    this->y_alignment = y_alignment;
}

/** Helper function that flushes the print & reference buffers
 */
void UIText::flushBuffers(){
    //Clear the print buffer
    if(this->print_buff != nullptr){
        for(unsigned int i = 0; i < num_lines; i++){
            memset(this->print_buff[i], '\0', this->chars_per_line + 1);
        }
    }

    //Clear the reference buffer
    if(this->ref_buff != nullptr){
        for(unsigned int i = 0; i < num_lines; i++){
            memset(this->ref_buff[i], '\0', this->chars_per_line + 1);
        }
    }

}

/** Gets the next break point (a tab or space character)
 * @param str The string we're checking the next break for
 * @return A char* pointing to the first instance of a break
 */
inline char* UIText::getNextBreak(char* str){
    char* space_break = strchr(str, ' ');
    char* tab_break   = strchr(str, '\t');

    if(space_break != nullptr && tab_break == nullptr){
        return space_break;
    }
    else if(tab_break != nullptr && space_break == nullptr){
        return tab_break;
    }
    else if(tab_break != nullptr && space_break != nullptr){
        if(space_break < tab_break){
            return space_break;
        }
        else if(tab_break < space_break){
            return tab_break;
        }
    }

    return nullptr;
}

/** Loads the next section of text into the ref buff
 */
void UIText::nextPage(){
    this->flushBuffers();

    for(unsigned int i = 0; i < this->num_lines && this->text_buff_cursor != nullptr; i++){
        for(unsigned int j = 0; j < this->chars_per_line && this->text_buff_cursor != nullptr;){
            //Don't allow a space at the start of a line
            if(j == 0){
                while(*this->text_buff_cursor == ' ' || *this->text_buff_cursor == '\t' || *this->text_buff_cursor == '-'){
                    this->text_buff_cursor++;
                }
            }

            //Get next break
            char* next_break = this->getNextBreak(text_buff_cursor+1);

            //Calculate remaining buffer & text space
            unsigned int remaining_buff_len = this->chars_per_line - j;
            unsigned int remaining_text_len = strlen(this->text_buff_cursor);

            //If there are no more breaks
            if(next_break == nullptr){
                //If we have enough space left
                if(remaining_buff_len >= remaining_text_len){
                    memcpy(ref_buff[i]+j, text_buff_cursor, remaining_text_len);
                    text_buff_cursor += remaining_text_len;
                }
                //If we don't have enough remaining buffer, and have not loaded text on this line
                else if(remaining_buff_len < remaining_text_len && j == 0){
                    memcpy(ref_buff[i]+j, text_buff_cursor, remaining_buff_len);
                    text_buff_cursor += remaining_buff_len;
                }
                break;
            }
            //If there is a line break, but we have enough room
            else if(next_break <= this->text_buff_cursor + remaining_buff_len){
                unsigned int copy_len = (next_break - text_buff_cursor) / sizeof(char);
                memcpy(ref_buff[i]+j, text_buff_cursor, copy_len);
                text_buff_cursor += copy_len;
                j += copy_len;
            }
            //If there is a line break, and have not loaded text on this line
            else if(next_break > this->text_buff_cursor + remaining_buff_len && j == 0){
                memcpy(ref_buff[i], text_buff_cursor, remaining_buff_len);
                text_buff_cursor += remaining_buff_len;
                break;
            }
            //If there is a line break, there's no room for the text, and have loaded text on this line
            else{
                break;
            }
        }

    }
}

/** If we've hit the end of text
 * @return If we've hit the end of text
 */
bool UIText::hitTextEnd(){
    if(text_buff_cursor != nullptr){
        return *text_buff_cursor == '\0';
    }

    return false;
}

/** If we've hit the end of the draw area
 * @return If we've hit the end of the draw area
 */
bool UIText::hitCharLimit(){
    return (this->chars_per_line - strlen(ref_buff[num_lines-1])) <= strlen(text_buff_cursor);
}

/** Draws this UIElement and all children UIElements
 * @param renderer The SDL_Renderer we're drawing to
 * @param delta The time passed since last draw (in ms)
 * @param camera_x The X location of the camera
 * @param camera_y The Y location of the camera
 */
void UIText::draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y){
    int width;
    int height;
    SDL_Rect draw_rect = this->draw_area;
    if(TTF_SizeText(this->font, "W", &width, &height)){
        fprintf(stderr, "Could not properly get the character size!\n");
        return;
    }
    //Height will remain constant across all lines
    draw_rect.h = height;

    //If we aren't scrolling, use the ref buff rather than print buff
    char** buff;
    if(this->scroll_speed == 0.0){
        buff = ref_buff;
    }
    else{
        buff = print_buff;
    }

    //Draw this element
    if(this->font != nullptr && this->print_buff != nullptr){
        unsigned int drawn_lines = 0;
        for(unsigned int i = 0; i < num_lines; i++){
            if(buff[i][0] != '\0'){
                drawn_lines++;
            }
        }

        if(this->y_alignment == ALIGNMENT::CENTER){
            draw_rect.y = draw_area.y + ((draw_area.h - (draw_rect.h * drawn_lines)) / 2);
        }

        //Still need to figure out line breaks in the function; maybe use a print_buff we populate w/ the normal print_buff,
        // then call this section repeatedly w/ the new buf once populated?
        for(unsigned int i = 0; i < num_lines; i++){
            //Set width of the rect
            draw_rect.w = strlen(buff[i]) * width;

            if(this->x_alignment == ALIGNMENT::CENTER){
                draw_rect.x = draw_area.x + ((draw_area.w - draw_rect.w) / 2);
            }

            SDL_Surface* surface = TTF_RenderUTF8_Blended(this->font, buff[i], this->font_color);
            if(surface != nullptr){
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_RenderCopy(renderer, texture, nullptr, &draw_rect);
                SDL_FreeSurface(surface);
                SDL_DestroyTexture(texture);
            }

            //Offset for the next object
            draw_rect.y += height;
        }
    }
}

/** Handles UIText processing (like scrolling text print)
 * @param delta The amount of time that has passed since last processing (in ms)
 */
void UIText::process(uint64_t delta, double step_size){
    //Scrolling the text from the internal text representation to the print_buffer
    if(this->print_buff != nullptr && this->ref_buff != nullptr && this->text != nullptr && this->scroll_speed != 0.0){
        //Find the first differing character (we're finished if we never hit a differing character)
        unsigned int line_num, char_num;
        unsigned int curr_num_chars = 0;
        bool finished = true;
        for(line_num = 0; line_num < this->num_lines; line_num++){
            curr_num_chars += strlen(print_buff[line_num]);
            for(char_num = 0; char_num < this->chars_per_line; char_num++){
                if(this->print_buff[line_num][char_num] != this->ref_buff[line_num][char_num]){
                    finished = false;
                    break;
                }
            }
            if(finished == false){
                break;
            }
        }

        if(!finished){
            this->timer += delta;
            unsigned int max_chars = timer * this->scroll_speed / 1000;

            for(; line_num < num_lines; line_num++){
                for(; char_num < chars_per_line && curr_num_chars < max_chars; char_num++){
                    //If we hit end-of-input for the line, move to end-of-line
                    if(this->ref_buff[line_num][char_num] == '\0'){
                        char_num = chars_per_line;
                        break;
                    }

                    //Copy the character
                    this->print_buff[line_num][char_num] = this->ref_buff[line_num][char_num];
                    curr_num_chars++;
                }
                //If we hit end-of-line, move to next line
                if(char_num == chars_per_line){
                    char_num = 0;
                }
            }
        }
    }
}