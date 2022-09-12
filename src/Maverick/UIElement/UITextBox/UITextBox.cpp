#include "./UITextBox.hpp"
#include "Maverick/Engine/Engine.hpp"

UITextBox::UITextBox(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                     int draw_layer, const char* font_path, char* text, float scroll_speed, int size, ALIGNMENT text_x_alignment,
                     ALIGNMENT text_y_alignment)
    : UIElement(name, view_x_offset, view_y_offset, view_width, view_height, draw_layer){
    this->type = UI_ELEMENT_TYPE::TEXT_BOX;

    int win_width, win_height;
    Engine* engine = Engine::getInstance();
    SDL_GetWindowSize(engine->getWindow(), &win_width, &win_height);

    unsigned int height_buff = 0, width_buff = 0;

    double text_view_x_offset = ((view_x_offset * win_width) + width_buff) / win_width;
    double text_view_y_offset = ((view_y_offset * win_height) + height_buff) / win_height;

    double text_view_width = ((view_width * win_width) - (width_buff * 2)) / win_width;
    double text_view_height = ((view_height * win_height) - (height_buff * 2)) / win_height;

    this->text = new UIText(name, text_view_x_offset, text_view_y_offset, text_view_width, text_view_height, draw_layer,
                            font_path, text, scroll_speed, size, text_x_alignment, text_y_alignment);
    
    this->addSubelement(this->text);
}

void UITextBox::setText(const char* text){
    this->text->setText(text);
}

void UITextBox::setFont(const char* font_path){
    this->text->setFont(font_path);
}

void UITextBox::setStyle(enum FONT_STYLE style){
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