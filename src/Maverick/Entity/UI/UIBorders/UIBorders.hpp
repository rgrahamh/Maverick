#ifndef UIBorders_H
#define UIBorders_H
#include "../UIElement.hpp"
#include "../UIText/UIText.hpp"

enum BORDER_TYPE{
    NO_BORDER = 0,
    TOP_BORDER = 1,
    BOTTOM_BORDER = 2,
    LEFT_BORDER = 4,
    RIGHT_BORDER = 8,
    ALL_BORDERS = TOP_BORDER | BOTTOM_BORDER | LEFT_BORDER | RIGHT_BORDER
};

class UIBorders: public UIElement{
    public:
        UIBorders(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                  int draw_layer, const char* border_pattern = "", BORDER_TYPE border_types = BORDER_TYPE::ALL_BORDERS);

        unsigned int getHeightBuff();
        unsigned int getWidthBuff();

        void setBorderPattern(const char* border_pattern);
        void setBorderType(BORDER_TYPE border_type);

        void addBorders();

        virtual void setViewSize(double view_width, double view_height) override;

    private:
        unsigned int height_buff;
        unsigned int width_buff;

        UIElement* borders[4];

        char* border_pattern;
        BORDER_TYPE border_type;
};

#endif