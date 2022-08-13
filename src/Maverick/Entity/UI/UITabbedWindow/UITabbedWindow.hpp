#ifndef UITABBEDWINDOW_H
#define UITABBEDWINDOW_H

#include "../UIElement.hpp"
#include "../UITextBox/UITextBox.hpp"

class UITabbedWindow : public UIElement{
    public:
        UITabbedWindow(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                       int draw_layer, double tab_height, const char* font_path, unsigned int font_size = 1, const char* border_pattern = "",
                       uint8_t border_type = BORDER_TYPE::NO_BORDER, uint32_t border_buff = 0);

		virtual void _process(uint64_t delta, unsigned int steps) override;
        virtual void _draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y) override;
		virtual void _action(Control* control) override;

        virtual void addElement(UIElement* element) override;

    private:
        int getTabNumber(Control* control);

        double tab_height;

        UIElement* selected_element;
        UITextBox draw_text;

};

#endif