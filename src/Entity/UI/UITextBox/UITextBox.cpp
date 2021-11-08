#include "./UITextBox.hpp"

extern TextureHash* texture_hash;

UITextBox::UITextBox(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                     unsigned int animation_num, int draw_layer, SDL_Window* window, char* font_path, char* text, float scroll_speed,
                     int point, ALIGNMENT text_x_alignment, ALIGNMENT text_y_alignment, char* border_pattern, uint8_t border_types)
    : UIElement(name, view_x_offset, view_y_offset, view_width, view_height, animation_num, draw_layer, UI_OBJECT_TYPE::TEXT_BOX, window){
    this->text = new UIText(name, view_x_offset, view_y_offset, view_width, view_height, animation_num, draw_layer, window, font_path, text, scroll_speed, point, text_x_alignment, text_y_alignment);

    this->subelements = new UIElementList;
    this->subelements->element = this->text;
    this->subelements->next = nullptr;

    addBorders(border_pattern, border_types);
}

/** Default destructor
 */
UITextBox::~UITextBox(){
    free(this->text);
    free(subelements);
}

void UITextBox::addBorders(char* border_pattern, uint8_t border_types){
    int win_width, win_height;
    SDL_GetWindowSize(this->window, &win_width, &win_height);

    if(border_pattern != nullptr && strlen(border_pattern) != 0){
        //The patterns we look for while building borders
        char* file_patterns[4] = {"_top.png", "_bottom.png", "_left.png", "_right.png"};
        char* border_names[4] = {"top_border", "bottom_border", "left_border", "right_border"};
        for(int i = 0; i < 4; i++){
            if(border_types & (1 << i)){
                //Make a new combined string, formatted <border_pattern><file_pattern> (as an example, "basic_border_top.png")
                int combined_len = strlen(file_patterns[i]) + strlen(border_pattern) + 1;
                char sprite_path[combined_len];
                strcpy(sprite_path, border_pattern);
                strcat(sprite_path, file_patterns[i]);
                sprite_path[combined_len - 1] = '\0';

                //Get the surface of the border (necessary to check if the texture can be loaded & get texture height/width)
                SDL_Surface* border_surface = texture_hash->get(sprite_path);

                if(border_surface != nullptr){
                    double border_width, border_height, border_x, border_y;
                    switch(1 << i){
                        case(BORDER_TYPE::TOP_BORDER):{
                            border_width = draw_area.w;
                            border_height = border_surface->h;
                            border_x = draw_area.x;
                            border_y = draw_area.y;
                            break;
                        }
                        case(BORDER_TYPE::BOTTOM_BORDER):{
                            border_width = draw_area.w;
                            border_height = border_surface->h;
                            border_x = draw_area.x;
                            border_y = draw_area.y + this->draw_area.h - border_surface->h;
                            break;
                        }
                        case(BORDER_TYPE::LEFT_BORDER):{
                            border_width = border_surface->w;
                            border_height = draw_area.h;
                            border_x = draw_area.x;
                            border_y = draw_area.y;
                            break;
                        }
                        case(BORDER_TYPE::RIGHT_BORDER):{
                            border_width = border_surface->w;
                            border_height = draw_area.h;
                            border_x = draw_area.x + this->draw_area.w - border_surface->w;
                            border_y = draw_area.y;
                            break;
                        }
                    }

                    UIElement* new_element = new UIElement(border_names[i], border_x / (double)win_width, border_y / (double)win_height,
                                                          border_width / (double)win_width, border_height / (double)win_height, 1, this->getDrawLayer() + 1,
                                                          UI_OBJECT_TYPE::GENERIC, this->window);
                    new_element->addSprite(0, sprite_path);
                    this->addElement(new_element);
                }
            }
        }
    }
}

/** Sets the text in the textbox
 * @param style The text's new contents
 */
void UITextBox::setText(const char* text){
    this->text->setText(text);
}

/** Sets the text's font
 * @param style The text's new font
 */
void UITextBox::setFont(const char* font_path){
    this->text->setFont(font_path);
}

/** Sets the text's style
 * @param style The text's new style
 */
void UITextBox::setStyle(uint8_t style){
    this->text->setStyle(style);
}

/** Sets the text's color
 * @param r The text's new red value
 * @param g The text's new green value
 * @param b The text's new blue value
 * @param a The text's new alpha value
 */
void UITextBox::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    this->text->setColor(r, g, b, a);
}

/** Sets the text's scrolling speed
 * @param scroll_speed The speed at which the text will scroll
 */
void UITextBox::setScrollSpeed(float scroll_speed){
    this->text->setScrollSpeed(scroll_speed);
}

/** Sets the text's horizontal alignment
 * @param y_alignment The desired vertical alignment
 */
void UITextBox::setXAlignment(ALIGNMENT x_alignment){
    this->text->setXAlignment(x_alignment);
}

/** Sets the text's vertical alignment
 * @param y_alignment The desired vertical alignment
 */
void UITextBox::setYAlignment(ALIGNMENT y_alignment){
    this->text->setYAlignment(y_alignment);
}