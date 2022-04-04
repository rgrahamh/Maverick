#ifndef FONT_H
#define FONT_H

#include "../Global/Global.hpp"

#include <unordered_set>
#include <string>
#include <SDL2/SDL.h>

#define MAX_CHARS 256

#define NUM_STYLES 3
enum FONT_STYLE{
    STANDARD_STYLE = 0,
    BOLD_STYLE,
    ITALIC_STYLE
};

class Font{
    public:
        Font(const char* name);
        void setCharacter(unsigned char value, SDL_Surface* surface, enum FONT_STYLE = STANDARD_STYLE);
        void setStyle(enum FONT_STYLE);

        SDL_Surface* getCharacter(unsigned char val);
        SDL_Surface* getCharacter(unsigned char value, enum FONT_STYLE style);

        int serialize(FILE* file, std::unordered_set<std::string> font_set);

    private:
        char* name;
        FONT_STYLE style;
        SDL_Surface* typesetter[NUM_STYLES][MAX_CHARS];
        uint8_t num_chars[NUM_STYLES];
};

#endif