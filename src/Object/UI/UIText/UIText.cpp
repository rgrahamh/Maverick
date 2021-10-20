#include "UIText.hpp"

/** UIText constructor (for viewport calcs, 1.0 is one screen width/height)
 * @param name The name of the UIElement
 * @param view_x_offset The viewport X offset of the UIElement
 * @param view_y_offset The viewport X offset of the UIElement
 * @param view_width The viewport width of the UIElement
 * @param view_height The viewport height of the UIElement
 * @param animation_num The animation number of the UIElement (use for multiple would be blinking cursors)
 * @param draw_layer The draw layer of the UIElement (all child elements will be drawn directly on top)
 * @param obj_type The object type (a UI_OBJECT_TYPE)
 * @param window The current window (used for viewport calculation)
 * @param text The text to display
 * @param font_path The filepath to the .ttf file you wish to use
 * @param point The point (size) of the text
 * @param scroll_speed The speed at which text scrolls (in units of chars/sec)
 */
UIText::UIText(const char* name, float view_x_offest, float view_y_offset, float view_width, float view_height, unsigned int animation_num, int draw_layer, SDL_Window* window, const char* font_path, const char* text, unsigned int point, float scroll_speed)
    : UIElement(name, view_x_offest, view_y_offset, view_width, view_height, animation_num, draw_layer, UI_OBJECT_TYPE::TEXT, window){
    this->point = point;
    this->scroll_speed = scroll_speed;

    this->num_lines = 0;
    this->chars_per_line = 0;

    this->buff = nullptr;
    this->text = nullptr;
    this->font = nullptr;

    this->setFont(font_path);
    this->setPoint(point);

    this->back_color = SDL_Color{0xFF, 0xFF, 0xFF, 0x00};
    this->font_color = SDL_Color{0x00, 0x00, 0x00, 0xFF};

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
    if(this->buff != nullptr){
        free(this->buff);
    }
}

/** Sets the text's contents 
 * @param text The text you wish to use (will make a local copy of the text)
 */
void UIText::setText(const char* text){
    //Reset the text timer (since we have new text)
    this->timer = 0.0;

    unsigned int len = strlen(text);

    if(this->text != nullptr){
        free(this->text);
    }
    this->text = (char*)malloc(len + 1);

    if(this->buff != nullptr){
        for(unsigned int i = 0; i < num_lines; i++){
            memset(this->buff[i], '\0', this->chars_per_line + 1);
        }
    }

    for(size_t i = 0; i < len; i++){
        this->text[i] = text[i];
    }
    this->text[len] = '\0';
}

/** Sets the text's font (call upon change of font or point)
 * @param font_path The path to a .ttf you wish to use
 */
void UIText::setFont(const char* font_path){
    if(font_path != nullptr){
        this->setStyle(TTF_STYLE_NORMAL);
        if(this->font != nullptr){
            TTF_CloseFont(this->font);
        }

        this->font_path = font_path;
        this->font = TTF_OpenFont(font_path, point);
        if(font == nullptr){
            fprintf(stderr, "Could not open texture: %s\n", TTF_GetError());
        }
    }
    else{
        fprintf(stderr, "Failed to set font; font_path is null!\n");
    }
}

/** Sets the point (size) of the font
 * @param point The point (size) of the font
 */
void UIText::setPoint(unsigned int point){
    this->point = point;
    this->setFont(this->font_path);

    int char_width = 0;
    int char_height = 0;

    //Set line size and num of 
    if(TTF_SizeUTF8(this->font, "W", &char_width, &char_height) == 0){
        if(this->window != nullptr){

            int win_height = 0;
            int win_width = 0;
            SDL_GetWindowSize(this->window, &win_height, &win_width);

            //Free the old buffer
            if(this->buff != nullptr){
                for(unsigned int i = 0; i < this->num_lines; i++){
                    free(this->buff[i]);
                }
                free(this->buff);
            }

            //Calculate the buffer attributes
            this->num_lines = this->view_height * win_height / char_height;
            this->chars_per_line = this->view_width * win_width / char_width;

            //Allocate space for the new buffer
            this->buff = (char**)malloc(sizeof(char*) * num_lines);
            for(unsigned int i = 0; i < num_lines; i++){
                this->buff[i] = (char*)calloc(1, chars_per_line + 1);
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

/** Draws this UIElement and all children UIElements
 * @param renderer The SDL_Renderer we're drawing to
 * @param delta The time passed since last draw (in ms)
 * @param camera_x The X location of the camera
 * @param camera_y The Y location of the camera
 */
void UIText::draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y){
    int width;
    int height;
    SDL_Rect draw_rect = this->draw_area;
    if(TTF_SizeText(this->font, "W", &width, &height)){
        fprintf(stderr, "Could not properly get the character size!\n");
        return;
    }
    //Height will remain constant across all lines
    draw_rect.h = height;

    //Draw this element
    if(this->font != nullptr && this->buff != nullptr){
        //Still need to figure out line breaks in the function; maybe use a buff we populate w/ the normal buff,
        // then call this section repeatedly w/ the new buf once populated?
        for(unsigned int i = 0; i < num_lines; i++){
            //Set width of the rect
            draw_rect.w = strlen(this->buff[i]) * width;

            SDL_Surface* surface = TTF_RenderUTF8_Blended(this->font, this->buff[i], this->font_color);
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
void UIText::process(uint32_t delta){
    if(this->buff != nullptr && this->text != nullptr){
        //Scrolling the text from the internal text representation to the buffer
        //Determine the current buff len & text len
        unsigned int buff_len = 0;
        for(unsigned int i = 0; i < num_lines; i++){
            buff_len += strlen(buff[i]);
        }
        unsigned int text_len = strlen(this->text);
        unsigned int max_len = this->num_lines * this->chars_per_line;

        if(buff_len < text_len && buff_len != max_len){
            //If we aren't scrolling
            unsigned int remaining_len = max_len;

            //If we are scrolling
            if(this->scroll_speed > 0.0){
                //Calculate the max possible index based on the new time
                this->timer += delta;
                unsigned int max_index = this->timer * this->scroll_speed / 1000;
                remaining_len = (max_len < max_index)? max_len : max_index;
            }

            //This value is incremented whenever we're supposed to skip a character in the copy

            unsigned int text_line_stop = (text_len / chars_per_line) + 1;
            unsigned int line_stop = (num_lines < text_line_stop)? num_lines : text_line_stop;
            for(unsigned int i = buff_len / chars_per_line; i < line_stop; i++){
                unsigned int char_offset = i * chars_per_line;
                for(unsigned int j = 0; j + char_offset < remaining_len && j + char_offset < text_len; j++){
                    this->buff[i][j] = this->text[char_offset + j];
                }
            }
        }
    }
}