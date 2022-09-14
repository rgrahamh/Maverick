#include "UILevelPort.hpp"
#include "Maverick/Engine/Engine.hpp"

/** UIElement constructor (for viewport calcs, 1.0 is one screen width/height)
 * @param name The name of the UIElement
 * @param view_x_offset The viewport X offset of the UIElement
 * @param view_y_offset The viewport X offset of the UIElement
 * @param view_width The viewport width of the UIElement
 * @param view_height The viewport height of the UIElement
 * @param animation_num The animation number of the UIElement (use for multiple would be blinking cursors)
 * @param draw_layer The draw layer of the UIElement (all child elements will be drawn directly on top)
 * @param window The current window (used for viewport calculation)
 */
UILevelPort::UILevelPort(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height)
           : UIElement(name, view_x_offset, view_y_offset, view_width, view_height, 0){
}

/** Handles actions for this UILevelPort
 * @param event The event that has occurred
 */
void UILevelPort::action(Control* control){
    if(isMouseInside(control)){
        Engine* engine = Engine::getInstance();
        const MouseState* mouse = control->getMouse();
        float zoom_amount = mouse->scroll_wheel * 0.01;
        engine->setGlobalXScale(engine->getGlobalXScale() + zoom_amount);
        engine->setGlobalYScale(engine->getGlobalYScale() + zoom_amount);
    }
}