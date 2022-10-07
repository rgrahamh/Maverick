#ifndef UITABBEDWINDOW_H
#define UITABBEDWINDOW_H

#include "../UIElement.hpp"
#include "../UITextBox/UITextBox.hpp"

class UITabbedWindow : public UIElement{
    public:
        UITabbedWindow(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                       int draw_layer, double tab_height, const char* font_path, unsigned int font_size = 1);

        /** Called every frame for processing
         * @param delta The amount of time that has passed (in ms)
         * @param step_size The step size used for physics calcluation (probably not needed here)
         */
		virtual void _process(uint64_t delta, unsigned int steps) override;

        /** Draws this UIElement
         * @param delta The time passed since last draw (in ms)
         * @param draw_scope The draw scope of the tabbed window
         */
        virtual void drawSubelements(uint64_t delta, const SDL_Rect& draw_scope) override;

        /** Handles actions for this UIElement
         * @param control The control struct
         */
		virtual void _action(Control* control) override;

        /** Adds an element to the child element list
         * @param element The element to add
         */
        virtual void addSubelement(UIElement* element) override;

    private:
        /** Returns the tab number which the mouse is hovering over, or -1 if it's not over any tab
         * @param control The control struct
         */
        int getTabNumber(Control* control);

        double tab_height;

        int current_tab;
        UITextBox draw_text;

};

#endif