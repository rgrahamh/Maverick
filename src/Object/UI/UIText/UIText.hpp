#ifndef UITEXT_H
#define UITEXT_H

#include <SDL2/SDL_ttf.h>

#include "../UIElement.hpp"

class UIText : public UIElement{
    public:
        UIText(const char* name, float view_x_offest, float view_y_offset, float view_width, float view_height, unsigned int animation_num, int draw_layer, enum UI_OBJECT_TYPE obj_type, SDL_Window* window, const char* text = "", const char* font_path = nullptr, unsigned int point = 12, float scroll_speed = 0.0);
        virtual ~UIText();

        void draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y);
        void process(uint32_t delta);

        void setText(const char* text);
        void setFont(const char* font_path);
        void setStyle(uint8_t style);
        void setPoint(unsigned int point);
        void setFontColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
        void setBackgroundColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
        void setScrollSpeed(float scroll_speed);

    private:
        //The text & current printing buffer
        char* text;
        char* buff;

        //The font object & filepath
        TTF_Font* font;
        const char* font_path;

        //Font size
        unsigned int point;

        //The scroll speed (chars per second)
        float scroll_speed;
        uint32_t timer;

        //The foreground & background colors
        SDL_Color font_color;
        SDL_Color back_color;
};

#endif