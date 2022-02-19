#include "UIObjectFrame.hpp"

/** UIObjectFrame constructor (for viewport calcs, 1.0 is one screen width/height)
 * @param name The name of the UIElement
 * @param view_x_offset The viewport X offset of the UIElement
 * @param view_y_offset The viewport X offset of the UIElement
 * @param view_width The viewport width of the UIElement
 * @param view_height The viewport height of the UIElement
 * @param animation_num The animation number of the UIElement (use for multiple would be blinking cursors)
 * @param draw_layer The draw layer of the UIElement (all child elements will be drawn directly on top)
 * @param window The current window (used for viewport calculation)
 * @param reference The object we're using for reference in the frame
 * @param border_pattern The pattern for the border texture files you wish to use
 * @param border_types The border locations
 * @param border_buff How much buffer the text should be given (how far inset you should start the text box)
 */
UIObjectFrame::UIObjectFrame(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                             int draw_layer, Object* reference, char* border_pattern, uint8_t border_types, int border_buff)
    : UIElement(name, view_x_offset, view_y_offset, view_width, view_height, draw_layer){
    this->type = UI_ELEMENT_TYPE::OBJECT_FRAME;

    this->reference = reference;
    this->border_buff = border_buff;

    if(border_pattern != nullptr){
        this->borders = new UIBorders(name, view_x_offset, view_y_offset, view_width, view_height, draw_layer, border_pattern, border_types);
    }
    else{
        this->borders = nullptr;
    }
}

/** Default destructor
 */
UIObjectFrame::~UIObjectFrame(){
    if(borders != nullptr){
        delete this->borders;
    }
}

/** Draws this UIObjectFrame
 * @param renderer The SDL_Renderer we're drawing to
 * @param delta The time passed since last draw (in ms)
 * @param camera_x The X location of the camera
 * @param camera_y The Y location of the camera
 */
void UIObjectFrame::draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y){
    //Draw background
    if(active_animation != nullptr){
        this->active_animation->draw(renderer, delta, camera_x, camera_y);
    }

    //Draw reference
    if(this->reference != nullptr &&
       SDL_RenderCopy(renderer, this->reference->getSprite()->texture, NULL, &draw_area)){
        printf(SDL_GetError());
    }

    //Draw borders
    if(this->borders != nullptr){
        this->borders->_draw(renderer, delta, camera_x, camera_y);
    }
}

/** Sets the reference object to draw
 * @param reference The reference object being drawn in the frame
 */ 
void UIObjectFrame::setReference(Object* reference){
    this->reference = reference;
}