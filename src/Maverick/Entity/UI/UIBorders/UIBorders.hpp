#ifndef UIBorders_H
#define UIBorders_H
#include "../UIElement.hpp"
#include "../UIText/UIText.hpp"

enum BORDER_TYPE{
    TOP_BORDER = 1,
    BOTTOM_BORDER = 2,
    LEFT_BORDER = 4,
    RIGHT_BORDER = 8,
    ALL_BORDERS = TOP_BORDER | BOTTOM_BORDER | LEFT_BORDER | RIGHT_BORDER
};

class UIBorders: public UIElement{
    public:
        UIBorders(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                  int draw_layer, char* border_pattern = "", uint8_t border_types = TOP_BORDER | BOTTOM_BORDER | LEFT_BORDER | RIGHT_BORDER);
        ~UIBorders();

        unsigned int getHeightBuff();
        unsigned int getWidthBuff();

        void addBorders(char* border_pattern, uint8_t border_types);

        virtual void draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y);

    private:
        UIElement* borders[4];
        unsigned int height_buff;
        unsigned int width_buff;
};

#endif