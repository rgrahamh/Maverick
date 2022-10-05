#ifndef TEXT_H
#define TEXT_H

#include "Maverick/Font/Font.hpp"

#include <inttypes.h>
#include <SDL2/SDL.h>

enum ALIGNMENT{
    CENTER_ALIGN,
    STANDARD_ALIGN
};

class Text{
    public:
        Text();

        /** Text Default Destructor
         */
        ~Text();

        void draw(uint64_t delta, unsigned int x_origin, unsigned int y_origin);
        void process(uint64_t delta, unsigned int steps);

        /** Sets the text's contents 
         * @param text The text you wish to use (will make a local copy of the text, in case the referenced text is unloaded at some point)
         */
        void setText(const char* text);

        /** Sets the text's font (call upon change of font or size)
         * @param font_name The path to a .ttf you wish to use
         */
        void setFont(const char* font_name);

        /** Sets the style of the font
         * @param style Any number of TTF_STYLE_* attrs OR'd together
         */
        void setStyle(enum FONT_STYLE style);

        /** Sets the size of the font
         * @param size The point (size) of the font
         */
        void setSize(unsigned int size);

        /** Sets the color of font
         * @param r The red value of the font
         * @param g The green value of the font
         * @param b The blue value of the font
         * @param a The alpha value of the font
         */
        void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);

        /** Sets the horizontal alignment
         * @param horizontal_align The horizontal alignment
         */
        void setHorizontalAlignment(ALIGNMENT horizontal_align);

        /** Sets the vertical alignment
         * @param vertical_align The vertical alignment
         */
        void setVerticalAlignment(ALIGNMENT vertical_align);

        /** Sets the text scroll speed
         * @param scroll_speed The speed of text scroll speed (0.0 means it's instant, otherwise it's in units of chars/sec)
         */
        void setScrollSpeed(float scroll_speed);

    private:
        //The text
        char* text;

        //The current location of the buffer's stopping point
        char* buf_cursor;

        //The font object & filepath
        Font* font;

        //Text style (italics, bold, etc.)
        enum FONT_STYLE style;

        //The scroll speed (chars per second)
        float scroll_speed;
        uint32_t timer;

        //The foreground & background colors
        SDL_Color font_color;

        //The text's horizontal alignment
        ALIGNMENT horizontal_align;

        //The text's vertical alignment
        ALIGNMENT vertical_align;
};

#endif