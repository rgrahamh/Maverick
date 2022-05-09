#include "./Font.hpp"

#include "../Engine/Engine.hpp"
extern Engine* engine;

Font::Font(const char* name){
    this->name = StrDeepCopy(name);
    memset(this->typesetter, 0, NUM_STYLES * MAX_CHARS * sizeof(SDL_Surface*));
    memset(this->num_chars, 0, NUM_STYLES);
}

SDL_Texture* Font::getCharacterTexture(unsigned char value, uint8_t style, uint8_t size){
    //If the texture exists
    if(type_textures[style][value].find(size) != type_textures[value][style].end()){
        return type_textures[style][value][size];
    }
    
    //Create a new, properly-sized surface to make a texture from
    SDL_Surface* ref_surface = typesetter[style][value];
    if(ref_surface == nullptr){
        return nullptr;
    }

    SDL_Rect new_rect;
    new_rect.w = ref_surface->w * engine->getGlobalXScale() * size;
    new_rect.h = ref_surface->h * engine->getGlobalYScale() * size;
    new_rect.x = 0;
    new_rect.y = 0;
    SDL_Surface* new_surface = SDL_CreateRGBSurface(0, new_rect.w, new_rect.h, ref_surface->format->BitsPerPixel,
                                                    ref_surface->format->Rmask, ref_surface->format->Gmask,
                                                    ref_surface->format->Bmask, ref_surface->format->Amask);
    SDL_BlitSurface(ref_surface, nullptr, new_surface, &new_rect);

    SDL_Texture* new_texture = SDL_CreateTextureFromSurface(engine->getCamera()->getRenderer(), new_surface);
    
    //Free the surface, now that the texture is created
    SDL_FreeSurface(new_surface);

    //Store & return the new texture
    type_textures[style][value][size] = new_texture;
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