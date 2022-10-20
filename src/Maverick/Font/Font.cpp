#include "./Font.hpp"
#include "Maverick/Engine/Engine.hpp"
#include "Maverick/FileHandler/FileHandler.hpp"

Font::Font(const char* name, uint16_t spacing){
    this->name = StrDeepCopy(name);
    memset(this->typesetter, 0, NUM_STYLES * MAX_CHARS * sizeof(SDL_Surface*));
    memset(this->num_chars, 0, NUM_STYLES);
    memset(this->char_height, 0, NUM_STYLES * sizeof(this->char_height));
    this->spacing = spacing;
}

unsigned int Font::getCharHeight(FONT_STYLE style){
    if(unlikely(this->char_height[style] == 0)){
        for(int i = 0; i < 256; i++){
            Animation* animation = this->typesetter[style][i];
            if(animation != nullptr){
                Sprite* sprite = animation->getSprite();
                if(sprite != nullptr && sprite->surface != nullptr){
                    this->char_height[style] = sprite->surface->h * Engine::getInstance()->getNativeScale();
                    break;
                }
            }
        }
    }

    return this->char_height[style];
}

SDL_Texture* Font::getCharacterTexture(unsigned char value, FONT_STYLE style){
    //If the texture exists
    Sprite* sprite = typesetter[style][value]->getSprite();
    if(sprite->texture != nullptr){
        return sprite->texture;
    }

    SDL_Texture* new_texture = SDL_CreateTextureFromSurface(Engine::getInstance()->getRenderer(), typesetter[style][value]->getSprite()->surface);
    
    //Store & return the new texture
    sprite->texture = new_texture;
    return new_texture;
}

SDL_Surface* Font::getCharacterSurface(unsigned char value, FONT_STYLE style){
    return this->typesetter[style][value]->getSprite()->surface;
}

uint16_t Font::getSpacing(){
    return this->spacing;
}

void Font::setCharacter(unsigned char value, Animation* animation, FONT_STYLE style){
    this->typesetter[style][value] = animation;
    this->num_chars[style]++;
}

void Font::setSpacing(uint16_t spacing){
    this->spacing = spacing;
}

void Font::removeCharacter(unsigned char value, enum FONT_STYLE style){
    this->typesetter[style][value] = nullptr;
    this->num_chars[style]--;
}

int Font::serializeSurfaces(FILE* file, SerializeSet& serialize_set){
    if(file == nullptr){
        return -1;
    }

    uint8_t resource_type = RESOURCE_TYPE::FONT;
    fwrite(&resource_type, sizeof(resource_type), 1, file);

    uint16_t identifier_len = strlen(name);
    WriteVar(identifier_len, file);
    fwrite(name, 1, identifier_len, file);

    WriteVar(this->spacing, file);

    uint8_t num_styles = 0;
    for(unsigned int i = 0; i < NUM_STYLES; i++){
        if(num_chars[i] > 0){
            num_styles++;
        }
    }

    fwrite(&num_styles, sizeof(num_styles), 1, file);
    for(unsigned int i = 0; i < NUM_STYLES; i++){
        if(num_chars[i] > 0){
            uint8_t font_style = i;
            fwrite(&font_style, sizeof(font_style), 1, file);
            fwrite(&this->num_chars[i], sizeof(num_chars[i]), 1, file);

            for(unsigned int j = 0; j < MAX_CHARS; j++){
                if(typesetter[font_style][j] != nullptr){
                    typesetter[font_style][j]->serializeAssets(file, serialize_set);
                }
            }
        }
    }

    return 0;
}

int Font::serializeData(FILE* file){
    if(file == nullptr){
        return -1;
    }

    for(unsigned int i = 0; i < NUM_STYLES; i++){
        if(num_chars[i] > 0){
            uint8_t font_style = i;
            for(unsigned int j = 0; j < MAX_CHARS; j++){
                if(typesetter[font_style][j] != nullptr){
                    char new_char = j;
                    fwrite(&new_char, sizeof(new_char), 1, file);
                    typesetter[font_style][j]->serializeData(file);
                }
            }
        }
    }

    return 0;
}