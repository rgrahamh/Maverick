#include "./UITextBox.hpp"

UITextBox::UITextBox(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height, unsigned int animation_num, int draw_layer, SDL_Window* window, char* font_path, char* text, float scroll_speed, int point, ALIGNMENT text_x_alignment, ALIGNMENT text_y_alignment)
    : UIElement(name, view_x_offset, view_y_offset, view_width, view_height, animation_num, draw_layer, UI_OBJECT_TYPE::TEXT_BOX, window){
    this->text = new UIText(name, view_x_offset, view_y_offset, view_width, view_height, animation_num, draw_layer, window, font_path, text, scroll_speed, point, text_x_alignment, text_y_alignment);

    this->subelements = new UIElementList;
    this->subelements->element = this->text;
    this->subelements->next = nullptr;
}

/** Default destructor
 */
UITextBox::~UITextBox(){
    free(this->text);
    free(subelements);
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