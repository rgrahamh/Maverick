#include "./Font.hpp"

Font::Font(const char* name){
    this->name = StrDeepCopy(name);
    this->style = FONT_STYLE::NORMAL;
    memset(this->typesetter, 0, NUM_STYLES * 256);
}

SDL_Surface* Font::getCharacter(char value){
    return this->typesetter[this->style][value];
}

void Font::setStyle(enum FONT_STYLE style){
    this->style = style;
}

void Font::setCharacter(char value, SDL_Surface* surface, enum FONT_STYLE style){
    this->typesetter[style][value] = surface;
}