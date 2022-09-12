#ifndef UITEXTBOX_H
#define UITEXTBOX_H
#include "../UIElement.hpp"
#include "../UIText/UIText.hpp"
#include "../UIBorders/UIBorders.hpp"

class UITextBox : public UIElement{
    public:
        /** UITextBox constructor (for viewport calcs, 1.0 is one screen width/height)
         * @param name The name of the UIElement
         * @param view_x_offset The viewport X offset of the UIElement
         * @param view_y_offset The viewport X offset of the UIElement
         * @param view_width The viewport width of the UIElement
         * @param view_height The viewport height of the UIElement
         * @param animation_num The animation number of the UIElement (use for multiple would be blinking cursors)
         * @param draw_layer The draw layer of the UIElement (all child elements will be drawn directly on top)
         * @param font_path The path to the font you want to use for the text
         * @param text The text you wish to display in the box
         * @param scroll_speed The scroll speed of the text
         * @param point The point size of the text
         * @param text_x_alignment The horizontal alignment strategy
         * @param text_y_alignment The vertical alignment strategy
         */
        UITextBox(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                  int draw_layer, const char* font_path, char* text = "", float scroll_speed = 0.0, int size = 1,
                  ALIGNMENT text_x_alignment = ALIGNMENT::STANDARD_ALIGN, ALIGNMENT text_y_alignment = ALIGNMENT::CENTER_ALIGN);

        /** Sets the text in the textbox
         * @param style The text's new contents
         */
        void setText(const char* text);

        /** Sets the text's font
         * @param style The text's new font
         */
        void setFont(const char* font_path);

        /** Sets the text's style
         * @param style The text's new style
         */
        void setStyle(enum FONT_STYLE style);

        /** Sets the text's color
         * @param r The text's new red value
         * @param g The text's new green value
         * @param b The text's new blue value
         * @param a The text's new alpha value
         */
        void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);

        /** Sets the text's scrolling speed
         * @param scroll_speed The speed at which the text will scroll
         */
        void setScrollSpeed(float scroll_speed);

        /** Sets the text's horizontal alignment
         * @param y_alignment The desired vertical alignment
         */
        void setXAlignment(ALIGNMENT x_alignment);

        /** Sets the text's vertical alignment
         * @param y_alignment The desired vertical alignment
         */
        void setYAlignment(ALIGNMENT y_alignment);

    private:
        UIText* text;

};

#endif