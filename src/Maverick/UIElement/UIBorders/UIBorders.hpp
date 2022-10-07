#ifndef UIBorders_H
#define UIBorders_H

#include "Maverick/UIElement/UIElement.hpp"

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
        /** UIBorders constructor (for viewport calcs, 1.0 is one screen width/height)
         * @param name The name of the UIElement
         * @param view_x_offset The viewport X offset of the UIElement
         * @param view_y_offset The viewport X offset of the UIElement
         * @param view_width The viewport width of the UIElement
         * @param view_height The viewport height of the UIElement
         * @param draw_layer The draw layer of the UIElement (all child elements will be drawn directly on top)
         * @param border_pattern The pattern for the border texture files you wish to use
         * @param border_types The border locations
         */
        UIBorders(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                  int draw_layer, const char* border_pattern = "", BORDER_TYPE border_types = BORDER_TYPE::ALL_BORDERS);

        /** Sets the border pattern
         * @param border_pattern The border pattern to use
         */
        void setBorderPattern(const char* border_pattern);

        /** Sets the border type
         * @param border_type The type of border to use
         */
        void setBorderType(BORDER_TYPE border_type);

        /** Creates & adds borders based upon the border_pattern & border_types
         * @param border_pattern The file pattern to match border files to (ex. "<border_pattern>[_top | _bottom | _left | _right].bmp")
         * @param border_types The types of borders you want (bitwise or of zero to all BORDER_TYPE fields, depending upon which sides you want the borders)
         */
        void addBorders();

        /** Draws the borders
         * @param delta The time passed since last draw (in ms)
         * @param draw_scope The area that the UIElement is allowed to draw in
         */
        virtual void draw(uint64_t delta, const SDL_Rect& draw_scope);

    private:
        UIElement* borders[4];

        char* border_pattern;
        BORDER_TYPE border_type;
};

#endif