#ifndef UIOBJECTFRAME_H
#define UIOBJECTFRAME_H

#include "../UIElement.hpp"
#include "../../Object/Object.hpp"

class UIObjectFrame : public UIElement{
    public:
        UIObjectFrame(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height, unsigned int animation_num, int draw_layer, SDL_Window* window, Object* reference = nullptr);
        ~UIObjectFrame() = default;

        virtual void draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y);

        void setReference(Object* reference);

    private:
        Object* reference;
};

#endif