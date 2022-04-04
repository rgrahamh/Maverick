#ifndef UITEXTBOX_H
#define UITEXTBOX_H
#include "../UIElement.hpp"
#include "../UIText/UIText.hpp"
#include "../UIBorders/UIBorders.hpp"

class UITextBox : public UIElement{
    public:
        UITextBox(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                  int draw_layer, char* font_path, char* text = "", float scroll_speed = 0.0, int point = 6,
                  ALIGNMENT text_x_alignment = ALIGNMENT::STANDARD_ALIGN, ALIGNMENT text_y_alignment = ALIGNMENT::CENTER_ALIGN,
                  char* border_pattern = "", uint8_t border_types = TOP_BORDER | BOTTOM_BORDER | LEFT_BORDER | RIGHT_BORDER,
                  int border_buff = 0);
        ~UITextBox();

        void setText(const char* text);
        void setFont(const char* font_path);
        void setStyle(uint8_t style);
        void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
        void setScrollSpeed(float scroll_speed);
        void setXAlignment(ALIGNMENT x_alignment);
        void setYAlignment(ALIGNMENT y_alignment);

        virtual void draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y);

    private:
        UIText* text;
        UIBorders* borders;

};

#endif