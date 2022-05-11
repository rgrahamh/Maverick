#include "./Font.hpp"

#include "../Engine/Engine.hpp"
extern Engine* engine;

Font::Font(const char* name){
    this->name = StrDeepCopy(name);
    memset(this->typesetter, 0, NUM_STYLES * MAX_CHARS * sizeof(SDL_Surface*));
    memset(this->num_chars, 0, NUM_STYLES);
}

SDL_Texture* Font::getCharacterTexture(unsigned char value, uint8_t style){
    //If the texture exists
    if(type_textures[style][value] != nullptr){
        return type_textures[style][value];
    }

    SDL_Texture* new_texture = SDL_CreateTextureFromSurface(engine->getCamera()->getRenderer(), typesetter[style][value]);
    
    //Store & return the new texture
    type_textures[style][value] = new_texture;
    return new_texture;
}

SDL_Surface* Font::getCharacterSurface(unsigned char value, uint8_t style){
    return this->typesetter[style][value];
}

void Font::setCharacter(unsigned char value, SDL_Surface* surface, uint8_t style){
    this->typesetter[style][value] = surface;
    this->num_chars[style]++;
}

void Font::removeCharacter(unsigned char value, enum FONT_STYLE style){
    this->typesetter[style][value] = nullptr;
    this->num_chars[style]--;
}

int Font::serialize(FILE* file){
    if(file == nullptr){
        return -1;
    }

    uint8_t resource_type = RESOURCE_TYPE::FONT;
    fwrite(&resource_type, sizeof(resource_type), 1, file);

    uint16_t identifier_len = strlen(name);
    uint16_t identifier_len_swapped = EndianSwap(&identifier_len);
    fwrite(&identifier_len_swapped, sizeof(identifier_len), 1, file);
    fwrite(name, 1, identifier_len, file);

    uint8_t num_styles = 0;
    for(int i = 0; i < NUM_STYLES; i++){
        if(num_chars[i] > 0){
            num_styles++;
        }
    }

    fwrite(&num_styles, sizeof(num_styles), 1, file);
    for(int i = 0; i < NUM_STYLES; i++){
        if(num_chars[i] > 0){
            uint8_t font_style;
            fwrite(&font_style, sizeof(font_style), 1, file);
            fwrite(&this->num_chars[i], sizeof(num_chars[i]), 1, file);

            for(int j = 0; j < MAX_CHARS; j++){
                if(typesetter[font_style][j] != nullptr){
                    char new_char = j;
                    fwrite(&new_char, sizeof(new_char), 1, file);
                    SerializeSurface(file, typesetter[font_style][j]);
                }
            }
        }
    }

    return 0;
}