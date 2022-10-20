#ifndef FONT_H
#define FONT_H

#include "Maverick/Global/Global.hpp"
#include "Maverick/Animation/Animation.hpp"

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <SDL2/SDL.h>

static const unsigned int MAX_CHARS = 256;
static const unsigned int NUM_STYLES = 3;

enum FONT_STYLE{
    STANDARD_STYLE = 0,
    BOLD_STYLE = 1,
    ITALIC_STYLE = 2
};

class Font{
    public:
        Font(const char* name, uint16_t spacing = 0);
        
        /** Sets a mapping from a character (inputted in the "value") to a surface in a given style (default standard)
         * @param value The character you wish to set up a mapping for
         * @param surface The surface you wish to map to the char
         * @param style The style set you'd like to add this character to
         */
        void setCharacter(unsigned char value, Animation* animation, FONT_STYLE style = FONT_STYLE::STANDARD_STYLE);

        /** Sets the character spacing (consistent between characters)
         * @param spacing The spacing that should be included between characters (in scaled pixels)
         */
        void setSpacing(uint16_t spacing);

        /** Gets the texture for a character
         * @param val The character you'd like to look up
         * @param style The style you're using
         * @return The texture for the character
         */
        SDL_Texture* getCharacterTexture(unsigned char val, FONT_STYLE style = FONT_STYLE::STANDARD_STYLE);

        /** Gets the surface for a character
         * @param val The character you'd like to look up
         * @param style The style you're using
         * @return The surface for the character
         */
        SDL_Surface* getCharacterSurface(unsigned char val, FONT_STYLE style = STANDARD_STYLE);
        
        /** Gets the character spacing for this font
         * @return The character spacing for this font
         */
        uint16_t getSpacing();

        /** Gets the height of a character in current config (assumes all letters are the same height)
         * @return The letter height, or 0 if no letters were found
         */
        unsigned int getCharHeight(FONT_STYLE style);

        /** Removes a character from the map
         * @param value The character you'd like to clear out
         * @param style The style you'd like to clear the character from
         */
        void removeCharacter(unsigned char value, enum FONT_STYLE style);

        /** Serializes the Font to file
         * @param file The file we're supposed to write to
         * @param serialize_set The serialization set (makes sure that things aren't written twice)
         * @return -1 if the file we're passed isn't valid, 0 otherwise
         */
        int serializeSurfaces(FILE* file, SerializeSet& serialize_set);

        /** Serializes the Font to file
         * @param file The file we're supposed to write to
         * @return -1 if the file we're passed isn't valid, 0 otherwise
         */
        int serializeData(FILE* file);

    private:
        char* name;
        Animation* typesetter[NUM_STYLES][MAX_CHARS];
        uint8_t num_chars[NUM_STYLES];
        uint16_t spacing;
        unsigned int char_height[NUM_STYLES];
};

#endif