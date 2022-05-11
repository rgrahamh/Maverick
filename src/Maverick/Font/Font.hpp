#ifndef FONT_H
#define FONT_H

#include "../Global/Global.hpp"

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <SDL2/SDL.h>

#define MAX_CHARS 256

#define NUM_STYLES 3
enum FONT_STYLE{
    STANDARD_STYLE = 0,
    BOLD_STYLE = 1,
    ITALIC_STYLE = 2
};

class Font{
    public:
        Font(const char* name);
        void setCharacter(unsigned char value, SDL_Surface* surface, uint8_t style = STANDARD_STYLE);

        SDL_Texture* getCharacterTexture(unsigned char val, uint8_t style = STANDARD_STYLE);
        SDL_Surface* getCharacterSurface(unsigned char val, uint8_t style = STANDARD_STYLE);
        void removeCharacter(unsigned char value, enum FONT_STYLE style);

        int serialize(FILE* file);

    private:
        char* name;
        SDL_Surface* typesetter[NUM_STYLES][MAX_CHARS];
        uint8_t num_chars[NUM_STYLES];
        SDL_Texture* type_textures[NUM_STYLES][MAX_CHARS];
};

#endif