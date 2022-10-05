#include "Text.hpp"
#include "Maverick/Engine/Engine.hpp"

Text::~Text(){
    if(this->text != nullptr){
        free(this->text);
    }
}

void Text::setText(const char* text){
    //Reset the text timer (since we have new text)
    this->timer = 0.0;

    unsigned int len = strlen(text);

    //Allocate room for all buffers
    if(this->text != nullptr){
        free(this->text);
    }
    this->text = StrDeepCopy(text);
    this->buf_cursor = this->text;
}

void Text::setFont(const char* font_name){
    if(font_name != nullptr){
        this->font = Engine::getInstance()->getFont(font_name);
        if(font == nullptr){
            fprintf(stderr, "Could not find font: %s\n", font_name);
        }
    }
}

void Text::setSize(unsigned int size){
    this->size = size;
}

void Text::setStyle(enum FONT_STYLE style){
    this->style = style;
}

void Text::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    this->font_color = SDL_Color{r, g, b, a};
}

void Text::setScrollSpeed(float scroll_speed){
    this->scroll_speed = scroll_speed;
}


void Text::setHorizontalAlignment(ALIGNMENT horizontal_align){
    this->horizontal_align = horizontal_align;
}

void Text::setVerticalAlignment(ALIGNMENT vertical_align){
    this->vertical_align = vertical_align;
}