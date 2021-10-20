#ifndef UITEXT_H
#define UITEXT_H

#include <SDL2/SDL_ttf.h>

#include "../UIElement.hpp"

class UIText : public UIElement{
    public:
        UIText(const char* name, float view_x_offest, float view_y_offset, float view_width, float view_height, unsigned int animation_num, int draw_layer, SDL_Window* window, const char* font_path, const char* text = "", unsigned int point = 12, float scroll_speed = 0.0);
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

        //Sets the speed at which text scrolls
        //(0.0 means it's instant, otherwise it's in units of chars/sec)
        void setScrollSpeed(float scroll_speed);

    private:
        //The text & current printing buffer
        char* text;
        char** buff;

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
        SDL_Color back_color;
};

#endif