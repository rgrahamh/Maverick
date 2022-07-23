#ifndef UIOBJECTFRAME_H
#define UIOBJECTFRAME_H

#include "../UIElement.hpp"
#include "../../Object/Object.hpp"
#include "../UIBorders/UIBorders.hpp"

class UIObjectFrame : public UIElement{
    public:
        UIObjectFrame(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height, int draw_layer, Object* reference = nullptr, char* border_pattern = "", uint8_t border_types = BORDER_TYPE::ALL_BORDERS, int border_buff = 0);
        ~UIObjectFrame();

        virtual void draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y) override;

        void setReference(Object* reference);

    private:
        Object* reference;
        SDL_Rect obj_draw_area;
        UIBorders* borders;
        int border_buff;
};

#endif