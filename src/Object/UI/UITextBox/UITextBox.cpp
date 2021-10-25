#include "./UITextBox.hpp"

UITextBox::UITextBox(const char* name, float view_x_offset, float view_y_offset, float view_width, float view_height, unsigned int animation_num, int draw_layer, SDL_Window* window, char* font_path, char* text, float scroll_speed, int point, ALIGNMENT text_x_alignment, ALIGNMENT text_y_alignment)
    : UIElement(name, view_x_offset, view_y_offset, view_width, view_height, animation_num, draw_layer, UI_OBJECT_TYPE::TEXT_BOX, window){
    this->text = new UIText(name, view_x_offset, view_y_offset, view_width, view_height, animation_num, draw_layer, window, font_path, text, scroll_speed, point, text_x_alignment, text_y_alignment);

    this->subelements = new UIElementList;
    this->subelements->element = this->text;
    this->subelements->next = nullptr;
}

UITextBox::~UITextBox(){
    free(this->text);
}

void UITextBox::setText(const char* text){
    this->text->setText(text);
}

void UITextBox::setFont(const char* font_path){
    this->text->setFont(font_path);
}

void UITextBox::setStyle(uint8_t style){
    this->text->setStyle(style);
}

void UITextBox::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    this->text->setColor(r, g, b, a);
}

void UITextBox::setScrollSpeed(float scroll_speed){
    this->text->setScrollSpeed(scroll_speed);
}

void UITextBox::setXAlignment(ALIGNMENT x_alignment){
    this->text->setXAlignment(x_alignment);
}

void UITextBox::setYAlignment(ALIGNMENT y_alignment){
    this->text->setYAlignment(y_alignment);
}