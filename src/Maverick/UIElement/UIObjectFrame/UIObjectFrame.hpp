#ifndef UIOBJECTFRAME_H
#define UIOBJECTFRAME_H

#include "../UIElement.hpp"
#include "Maverick/Object/Object.hpp"
#include "../UIBorders/UIBorders.hpp"

class UIObjectFrame : public UIElement{
    public:
        /** UIObjectFrame constructor (for viewport calcs, 1.0 is one screen width/height)
         * @param name The name of the UIElement
         * @param view_x_offset The viewport X offset of the UIElement
         * @param view_y_offset The viewport X offset of the UIElement
         * @param view_width The viewport width of the UIElement
         * @param view_height The viewport height of the UIElement
         * @param animation_num The animation number of the UIElement (use for multiple would be blinking cursors)
         * @param draw_layer The draw layer of the UIElement (all child elements will be drawn directly on top)
         * @param reference The object we're using for reference in the frame
         */
        UIObjectFrame(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height, int draw_layer, Object* reference = nullptr);
        /** Default destructor
         */
        ~UIObjectFrame();

        /** Draws this UIObjectFrame
         * @param renderer The SDL_Renderer we're drawing to
         * @param delta The time passed since last draw (in ms)
         * @param camera_x The X location of the camera
         * @param camera_y The Y location of the camera
         */
        virtual void draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y) override;

        /** Sets the reference object to draw
         * @param reference The reference object being drawn in the frame
         */ 
        void setReference(Object* reference);

    private:
        Object* reference;
};

#endif