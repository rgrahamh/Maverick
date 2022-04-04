#include "./Font.hpp"

Font::Font(const char* name){
    this->name = StrDeepCopy(name);
    this->style = FONT_STYLE::STANDARD_STYLE;
    memset(this->typesetter, 0, NUM_STYLES * MAX_CHARS);
    memset(this->num_chars, 0, NUM_STYLES);
}

SDL_Surface* Font::getCharacter(char value){
    return this->typesetter[this->style][value];
}

SDL_Surface* Font::getCharacter(char value, enum FONT_STYLE style){
    return this->typesetter[style][value];
}

void Font::setStyle(enum FONT_STYLE style){
    this->style = style;
}

void Font::setCharacter(char value, SDL_Surface* surface, enum FONT_STYLE style){
    this->typesetter[style][value] = surface;
    this->num_chars[style]++;
}

int Font::serialize(FILE* file, std::unordered_set<std::string> font_set){
    if(font_set.find(this->name) == font_set.end()){
        return -1;
    }

    uint8_t num_styles = 0;
    for(int i = 0; i < NUM_STYLES; i++){
        if(num_chars[i] > 0){
            num_styles++;
        }
    }

    for(int i = 0; i < NUM_STYLES; i++){
        if(num_chars[i] > 0){
            uint8_t font_style = this->style;
            fwrite(&font_style, sizeof(font_style), 1, file);
            fwrite(&this->num_chars[i], sizeof(num_chars[i]), 1, file);

            for(int j = 0; j < MAX_CHARS; j++){
                if(typesetter[font_style][j] != nullptr){
                    SerializeSurface(file, typesetter[font_style][j]);
                }
            }
        }
    }
}