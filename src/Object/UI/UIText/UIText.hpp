#ifndef UITEXT_H
#define UITEXT_H

#include <SDL2/SDL_ttf.h>

#include "../UIElement.hpp"

enum ALIGNMENT{
    CENTER,
    NORMAL
};

class UIText : public UIElement{
    public:
        UIText(const char* name, float view_x_offest, float view_y_offset, float view_width, float view_height, unsigned int animation_num, int draw_layer, SDL_Window* window, const char* font_path, const char* text = "", float scroll_speed = 0.0, unsigned int point = 12, ALIGNMENT x_alignment = ALIGNMENT::NORMAL, ALIGNMENT y_alignment = ALIGNMENT::NORMAL);
        virtual ~UIText();

        void draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y);
        void process(uint32_t delta);

        //Set the displayed text
        void setText(const char* text);

        //Sets the font
        void setFont(const char* font_path);

        //Sets the style (strikethrough, bold, etc.)
        void setStyle(uint8_t style);

        //Sets the point (size)
        void setPoint(unsigned int point);

        //Sets the text color
        void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);

        //Sets the X alignment
        void setXAlignment(ALIGNMENT x_alignment);

        //Sets the Y alignment
        void setYAlignment(ALIGNMENT y_alignment);

        //Sets the speed at which text scrolls
        //(0.0 means it's instant, otherwise it's in units of chars/sec)
        void setScrollSpeed(float scroll_speed);

        //Updates to the next page's contents (for continued text on the next "page")
        void nextPage();

        //If we hit the end of the text
        bool hitTextEnd();

        //If we hit the character limit
        bool hitCharLimit();

    private:
        //Gets the next line break
        char* getNextBreak(char* str);

        //Flushes current print & ref buffers
        void flushBuffers();

        //The text
        char* text;
        char* text_buff_cursor;

        //The reference buffer (and where it is on the text)
        char** ref_buff;

        //The printing buffer
        char** print_buff;

        //The font object & filepath
        TTF_Font* font;
        const char* font_path;

        //Font size
        unsigned int point;

        //Text wrapping characteristics
        unsigned int chars_per_line;
        unsigned int num_lines;

        //The scroll speed (chars per second)
        float scroll_speed;
        uint32_t timer;

        //The foreground & background colors
        SDL_Color font_color;

        //The text's horizontal alignment
        ALIGNMENT x_alignment;

        //The text's vertical alignment
        ALIGNMENT y_alignment;
};

#endif