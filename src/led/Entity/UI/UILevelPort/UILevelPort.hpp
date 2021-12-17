#ifndef UILEVELPORT_H
#define UILEVELPORT_H
#include "../../../../Maverick/Entity/UI/UIElement.hpp"

class UILevelPort : public UIElement{
    public:
        UILevelPort(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height, SDL_Window* window);
        ~UILevelPort() = default;

        void action(Control* control);
};

#endif