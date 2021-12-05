#ifndef UI_SCREEN_BLIT_H
#define UI_SCREEN_BLIT_H

#include "../UIElement.hpp"

class UIScreenBlit: public UIElement{
    public:
        UIScreenBlit(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height, unsigned int animation_num, int draw_layer, SDL_Window* window);
        virtual ~UIScreenBlit() = default;

        virtual void draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y);
};


#endif