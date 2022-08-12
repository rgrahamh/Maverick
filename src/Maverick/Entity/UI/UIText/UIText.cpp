#include "UIText.hpp"
#include "../../../Engine/Engine.hpp"

/** UIText constructor (for viewport calcs, 1.0 is one screen width/height)
 * @param name The name of the UIElement
 * @param view_x_offset The viewport X offset of the UIElement
 * @param view_y_offset The viewport Y offset of the UIElement
 * @param view_width The viewport width of the UIElement
 * @param view_height The viewport height of the UIElement
 * @param animation_num The animation number of the UIElement (use for multiple would be blinking cursors)
 * @param draw_layer The draw layer of the UIElement (all child elements will be drawn directly on top)
 * @param text The text to display
 * @param font_path The filepath to the .ttf file you wish to use
 * @param scroll_speed The speed at which text scrolls (in units of chars/sec)
 * @param point The point (size) of the text
 * @param x_alignment The horizontal alignment of the text
 * @param y_alignment The vertical alignment of the text
 */
UIText::UIText(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
               int draw_layer, const char* font_name, const char* text, float scroll_speed, unsigned int size,
               ALIGNMENT x_alignment, ALIGNMENT y_alignment)
    : UIElement(name, view_x_offset, view_y_offset, view_width, view_height, draw_layer){
    this->type = UI_ELEMENT_TYPE::TEXT;

    //Setting the text's alignment
    this->x_alignment = x_alignment;
    this->y_alignment = y_alignment;

    this->view_height = view_height;
    this->view_width = view_width;

    this->size = size;
    this->scroll_speed = scroll_speed;

    this->print_buff = nullptr;
    this->ref_buff = nullptr;
    this->text = nullptr;
    this->font = nullptr;
    this->style = FONT_STYLE::STANDARD_STYLE;

    this->line_start_x = nullptr;
    this->num_lines = 0;

    this->setFont(font_name);
    this->setSize(size);

    //Default color is white
    this->font_color = SDL_Color{0xFF, 0xFF, 0xFF, 0xFF};

    this->setText(text);
}

/** UIText Destructor
 */
UIText::~UIText(){
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

    //Allocate room for all buffers
    if(this->text != nullptr){
        free(this->text);
    }
    this->text = StrDeepCopy(text);

    if(this->print_buff != nullptr){
        free(this->print_buff);
    }
    this->print_buff = (char*)calloc((len + 1) * 2, sizeof(char));

    if(this->ref_buff != nullptr){
        free(this->ref_buff);
    }
    this->ref_buff = (char*)malloc((len + 1) * 2);

    //Set buffer cursor to the start of the text
    this->text_buff_cursor = this->text;

    //Load the next chunk of content
    this->nextPage();
}

/** Sets the text's font (call upon change of font or size)
 * @param font_name The path to a .ttf you wish to use
 */
void UIText::setFont(const char* font_name){
    if(font_name != nullptr){
        this->font = Engine::getInstance()->getFont(font_name);
        if(font == nullptr){
            fprintf(stderr, "Could not find font: %s\n", font_name);
        }
    }
}

/** Sets the size of the font
 * @param size The point (size) of the font
 */
void UIText::setSize(unsigned int size){
    this->size = size;
}

/** Sets the style of the font
 * @param style Any number of TTF_STYLE_* attrs OR'd together
 */
void UIText::setStyle(enum FONT_STYLE style){
    this->style = style;
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

/** Gets the height of a letter in current config (assumes all letters are the same height)
 * @return The letter height, or 0 if no letters were found
 */
float UIText::getCharHeight(){
    for(int i = 0; i < 256; i++){
        SDL_Surface* character = font->getCharacterSurface(i, this->style);
        if(character != nullptr){
            return character->h * this->size * Engine::getInstance()->getGlobalYScale();
        }
    }

    return 0;
}

/** Loads the next section of text into the ref buff
 */
void UIText::nextPage(){
    if(this->text == nullptr || this->text_buff_cursor == nullptr || this->font == nullptr){
        return;
    }

    //Clear buffers
    if(print_buff != nullptr && strlen(print_buff) > 0){
        memset(print_buff, 0, strlen(print_buff));
    }
    if(ref_buff != nullptr && strlen(ref_buff) > 0){
        memset(ref_buff, 0, strlen(ref_buff));
    }

    unsigned int char_height = getCharHeight();
    if(char_height == 0){
        return;
    }

    this->num_lines = 1;

    char* ref_cursor = ref_buff;
    char* text_cursor = text;

    //Get max length & height
    const float max_line_len = this->getWidth();
    const float max_line_height = this->getHeight();

    unsigned int total_height = 0;
    unsigned int line_len = 0;
    unsigned int word_len = 0;
    bool printed = false;
    while(text_cursor[word_len] != '\0' && total_height < max_line_height){
        SDL_Surface* character = font->getCharacterSurface(text_cursor[word_len], this->style);
        if(character == nullptr){
            word_len++;
            continue;
        }

        Engine* engine = Engine::getInstance();
        unsigned int letter_width = character->w * this->size * engine->getGlobalXScale() + font->getSpacing() * engine->getGlobalXScale();
        line_len += letter_width;

        if(text_cursor[word_len] == ' ' || text_cursor[word_len] == '-' || text_cursor[word_len] == '\t' || text_cursor[word_len] == '\n'){
            if(text_cursor[word_len] == '\n'){
                printed = false;
                line_len = 0;
                num_lines++;
                total_height += char_height;
            }
            else{
                printed = true;
            }

            //Print
            memcpy(ref_cursor, text_cursor, ++word_len);
            text_cursor += word_len;
            ref_cursor += word_len;
            word_len = 0;
        }
        if(line_len > max_line_len){
            if(!printed){
                //Print & hyphenate
                if(word_len > 1){
                    word_len--;
                }
                memcpy(ref_cursor, text_cursor, word_len);
                text_cursor += word_len;
                ref_cursor += word_len;

                *ref_cursor = '-';
                ref_cursor++;
            }
            *ref_cursor = '\n';
            ref_cursor++;

            total_height += char_height;
            word_len = 0;
            line_len = 0;
            printed = false;

            num_lines++;
        }
        else{
            word_len++;
        }
    }
    if(word_len > 0){
        memcpy(ref_cursor, text_cursor, ++word_len);
        ref_cursor += word_len;
    }
    *ref_cursor = '\0';

    if(scroll_speed == 0){
        strcpy(this->print_buff, this->ref_buff);
    }

    if(this->line_start_x != nullptr){
        free(this->line_start_x);
    }
    this->line_start_x = (uint32_t*)calloc(sizeof(uint32_t), num_lines);

    Engine* engine = Engine::getInstance();
    uint32_t line_num = 0;
    for(int i = 0; ref_buff[i] != '\0' && line_num < num_lines; i++){
        if(this->x_alignment == ALIGNMENT::CENTER_ALIGN){
            unsigned int total_width = 0;
            for(; ref_buff[i] != '\0' && ref_buff[i] != '\n'; i++){
                SDL_Surface* letter = font->getCharacterSurface(ref_buff[i], this->style);
                if(letter != nullptr){
                    total_width += letter->w * engine->getGlobalXScale() + this->font->getSpacing() * engine->getGlobalXScale();
                }
            }

            this->line_start_x[line_num++] = this->x + ((this->getWidth() - total_width) / 2);
        }
        else{
            this->line_start_x[line_num++] = this->x;
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
bool UIText::hitDrawEnd(){
    return strlen(print_buff) == strlen(ref_buff);
}

/** Draws this UIElement and all children UIElements
 * @param renderer The SDL_Renderer we're drawing to
 * @param delta The time passed since last draw (in ms)
 * @param camera_x The X location of the camera
 * @param camera_y The Y location of the camera
 */
void UIText::draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y){
    if(print_buff == nullptr || font == nullptr){
        return;
    }

    unsigned int char_height = getCharHeight();
    if(char_height == 0){
        return;
    }

    int win_width, win_height;
    Engine* engine = Engine::getInstance();
    SDL_GetWindowSize(engine->getWindow(), &win_width, &win_height);

    //Set starting Y draw position
    uint32_t y_draw;
    if(this->y_alignment == ALIGNMENT::CENTER_ALIGN){
        y_draw = this->y + ((this->getHeight() / 2) - (this->num_lines * char_height / 2));
    }
    else{
        y_draw = this->y;
    }

    uint32_t line_num = 0;
    uint32_t x_draw;
    for(int i = 0; print_buff[i] != '\0' && line_num < num_lines; i++){
        //Set the line X draw position
        x_draw = line_start_x[line_num++];
        for(; print_buff[i] != '\0' && print_buff[i] != '\n'; i++){
            SDL_Texture* letter = font->getCharacterTexture(print_buff[i], this->style);
            if(letter != nullptr){
                SDL_Rect dst_rect;
                dst_rect.x = x_draw;
                dst_rect.y = y_draw;
                dst_rect.w = font->getCharacterSurface(print_buff[i], this->style)->w * this->size * engine->getGlobalXScale();
                dst_rect.h = char_height;
                if(dst_rect.x + dst_rect.w <= win_width){
                    SDL_RenderCopy(renderer, letter, nullptr, &dst_rect);
                }

                x_draw += dst_rect.w + font->getSpacing() * engine->getGlobalXScale();
            }
        }
        y_draw += char_height;
    }
}

/** Handles UIText processing (like scrolling text print)
 * @param delta The amount of time that has passed since last processing (in ms)
 */
void UIText::process(uint64_t delta, unsigned int steps){
    if(this->print_buff == nullptr || this->ref_buff == nullptr || this->scroll_speed == 0.0){
        return;
    }

    //Scrolling the text from the internal text representation to the print_buffer
    unsigned int ref_buff_len = strlen(this->ref_buff);
    unsigned int print_buff_len = strlen(this->print_buff);

    if(ref_buff_len != print_buff_len){
        this->timer += delta;
        unsigned int max_chars = timer * this->scroll_speed / 1000;

        for(unsigned int i = print_buff_len; i < max_chars && i < ref_buff_len; i++){
            print_buff[i] = ref_buff[i];
        }
    }
}