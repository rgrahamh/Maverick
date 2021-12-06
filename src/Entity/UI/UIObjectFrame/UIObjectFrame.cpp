#include "UIObjectFrame.hpp"

UIObjectFrame::UIObjectFrame(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height, unsigned int animation_num, int draw_layer, SDL_Window* window, Object* reference)
    : UIElement(name, view_x_offset, view_y_offset, view_width, view_height, 0, draw_layer, UI_OBJECT_TYPE::OBJECT_FRAME, window){
    this->reference = reference;
}

void UIObjectFrame::draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y){
    if(this->reference != nullptr && SDL_RenderCopy(renderer, this->reference->getSprite()->texture, NULL, &draw_area)){
        printf(SDL_GetError());
    }
}

/** Sets the reference object to draw
 * @param reference The reference object being drawn in the frame
 */ 
void UIObjectFrame::setReference(Object* reference){
    this->reference = reference;
}