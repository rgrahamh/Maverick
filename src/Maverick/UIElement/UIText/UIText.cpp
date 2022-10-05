#include "UIText.hpp"
#include "Maverick/Engine/Engine.hpp"

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

UIText::~UIText(){
    if(this->text != nullptr){
        free(this->text);
    }
    if(this->print_buff != nullptr){
        free(this->print_buff);
    }
}

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

void UIText::setFont(const char* font_name){
    if(font_name != nullptr){
        this->font = Engine::getInstance()->getFont(font_name);
        if(font == nullptr){
            fprintf(stderr, "Could not find font: %s\n", font_name);
        }
    }
}

void UIText::setSize(unsigned int size){
    this->size = size;
}

void UIText::setStyle(enum FONT_STYLE style){
    this->style = style;
}

void UIText::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    this->font_color = SDL_Color{r, g, b, a};
}

void UIText::setScrollSpeed(float scroll_speed){
    this->scroll_speed = scroll_speed;
}


void UIText::setXAlignment(ALIGNMENT x_alignment){
    this->x_alignment = x_alignment;
}

void UIText::setYAlignment(ALIGNMENT y_alignment){
    this->y_alignment = y_alignment;
}

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

    unsigned int char_height = font->getCharHeight(this->style);
    if(char_height == 0){
        return;
    }

    Engine* engine = Engine::getInstance();

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

        unsigned int letter_width = character->w * this->size * engine->getNativeScale() + font->getSpacing() * engine->getNativeScale();
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

    uint32_t line_num = 0;
    for(int i = 0; ref_buff[i] != '\0' && line_num < num_lines; i++){
        if(this->x_alignment == ALIGNMENT::CENTER_ALIGN){
            unsigned int total_width = 0;
            for(; ref_buff[i] != '\0' && ref_buff[i] != '\n'; i++){
                SDL_Surface* letter = font->getCharacterSurface(ref_buff[i], this->style);
                if(letter != nullptr){
                    total_width += letter->w * engine->getNativeScale() + this->font->getSpacing() * engine->getNativeScale();
                }
            }

            this->line_start_x[line_num++] = this->draw_area.x + ((this->getWidth() - total_width) / 2);
        }
        else{
            this->line_start_x[line_num++] = this->draw_area.x;
        }
    }
}

bool UIText::hitTextEnd(){
    if(text_buff_cursor != nullptr){
        return *text_buff_cursor == '\0';
    }

    return false;
}

bool UIText::hitDrawEnd(){
    return strlen(print_buff) == strlen(ref_buff);
}

/** Draws this UIElement and all children UIElements
 * @param renderer The SDL_Renderer we're drawing to
 * @param delta The time passed since last draw (in ms)
 * @param camera_x The X location of the camera
 * @param camera_y The Y location of the camera
 */
void UIText::draw(uint64_t delta, const SDL_Rect& draw_scope){
    if(print_buff == nullptr || font == nullptr){
        return;
    }

    unsigned int char_height = font->getCharHeight(this->style);
    if(char_height == 0){
        return;
    }

    int win_width, win_height;
    Engine* engine = Engine::getInstance();
    SDL_Renderer* renderer = engine->getRenderer();
    SDL_GetWindowSize(engine->getWindow(), &win_width, &win_height);

    //Set starting Y draw position
    uint32_t y_draw = draw_scope.y;
    if(this->y_alignment == ALIGNMENT::CENTER_ALIGN){
        y_draw += this->draw_area.y + ((this->getHeight() / 2) - (this->num_lines * char_height / 2));
    }
    else{
        y_draw += this->draw_area.y;
    }

    uint32_t line_num = 0;
    uint32_t x_draw;
    for(int i = 0; print_buff[i] != '\0' && line_num < num_lines; i++){
        //Set the line X draw position
        x_draw = line_start_x[line_num++] + draw_scope.x + this->view_x_offset * win_width;
        for(; print_buff[i] != '\0' && print_buff[i] != '\n'; i++){
            SDL_Texture* letter = font->getCharacterTexture(print_buff[i], this->style);
            if(letter != nullptr){
                SDL_Rect dst_rect;
                dst_rect.x = x_draw;
                dst_rect.y = y_draw;
                dst_rect.w = font->getCharacterSurface(print_buff[i], this->style)->w * this->size * engine->getNativeScale();
                dst_rect.h = char_height;
                if(dst_rect.x + dst_rect.w <= win_width){
                    SDL_RenderCopy(renderer, letter, nullptr, &dst_rect);
                }

                x_draw += dst_rect.w + font->getSpacing() * engine->getNativeScale();
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