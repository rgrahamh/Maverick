#include "UIObjectFrame.hpp"

UIObjectFrame::UIObjectFrame(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                             unsigned int animation_num, int draw_layer, SDL_Window* window, Object* reference,
                             char* border_pattern, uint8_t border_types, int border_buff)
    : UIElement(name, view_x_offset, view_y_offset, view_width, view_height, animation_num, draw_layer, UI_OBJECT_TYPE::OBJECT_FRAME, window){
    this->reference = reference;

    this->border_buff = border_buff;

    if(border_pattern != nullptr){
        this->borders = new UIBorders(name, view_x_offset, view_y_offset, view_width, view_height, 0, draw_layer,
                                    window, border_pattern, border_types);
    }
    else{
        this->borders = nullptr;
    }
}

UIObjectFrame::~UIObjectFrame(){
    if(borders != nullptr){
        delete this->borders;
    }
}

void UIObjectFrame::draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y){
    //Draw background
    if(active_animation < this->total_animation_num){
        this->animations[active_animation]->draw(renderer, delta, camera_x, camera_y);
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