#include "./UITextBox.hpp"

extern TextureHash* texture_hash;

UITextBox::UITextBox(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                     unsigned int animation_num, int draw_layer, SDL_Window* window, char* font_path, char* text, float scroll_speed,
                     int point, ALIGNMENT text_x_alignment, ALIGNMENT text_y_alignment, char* border_pattern, uint8_t border_types, int border_buff)
    : UIElement(name, view_x_offset, view_y_offset, view_width, view_height, animation_num, draw_layer, UI_OBJECT_TYPE::TEXT_BOX, window){
    int window_width, window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);

    double text_view_x_offset = ((view_x_offset * window_width) + border_buff) / window_width;
    double text_view_y_offset = ((view_y_offset * window_height) + border_buff) / window_height;

    double text_view_width = ((view_width * window_width) - (border_buff * 2)) / window_width;
    double text_view_height = ((view_height * window_height) - (border_buff * 2)) / window_height;

    this->text = new UIText(name, text_view_x_offset, text_view_y_offset, text_view_width, text_view_height,
                            animation_num, draw_layer, window, font_path, text, scroll_speed,
                            point, text_x_alignment, text_y_alignment);

    this->subelements = nullptr;

    this->borders = new UIBorders(name, view_x_offset, view_y_offset, view_width, view_height, 0, draw_layer,
                                  window, border_pattern, border_types, border_buff);
}

/** Default destructor
 */
UITextBox::~UITextBox(){
    if(this->borders != nullptr){
        delete this->borders;
    }

    delete this->text;
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


/** Draws this UITextBox
 * @param renderer The SDL_Renderer we're drawing to
 * @param delta The time passed since last draw (in ms)
 * @param camera_x The X location of the camera
 * @param camera_y The Y location of the camera
 */
void UITextBox::draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y){
    //Draw the textbox background
    if(active_animation < animation_num){
        this->animations[active_animation]->draw(renderer, delta, camera_x, camera_y);
    }

    //Draw the text
    if(this->text != nullptr){
        this->text->_draw(renderer, delta, camera_x, camera_y);
    }

    //Draw the borders
    if(this->borders != nullptr){
        this->borders->_draw(renderer, delta, camera_x, camera_y);
    }

}