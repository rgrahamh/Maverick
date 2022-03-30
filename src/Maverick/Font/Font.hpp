#ifndef FONT_H
#define FONT_H

#include "../Utility/Utility.hpp"

#include <SDL2/SDL.h>

#define NUM_STYLES 3
enum FONT_STYLE{
    NORMAL,
    BOLD,
    ITALIC
};

class Font{
    public:
        Font(const char* name);
        void setCharacter(char value, SDL_Surface* surface, enum FONT_STYLE = NORMAL);
        void setStyle(enum FONT_STYLE);

        SDL_Surface* getCharacter(char val);

    private:
        char* name;
        FONT_STYLE style;
        SDL_Surface* typesetter[NUM_STYLES][256];
};

#endif