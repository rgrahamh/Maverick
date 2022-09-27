#include "UIObjectFrame.hpp"
#include "Maverick/Engine/Engine.hpp"

UIObjectFrame::UIObjectFrame(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                             int draw_layer, Object* reference)
    : UIElement(name, view_x_offset, view_y_offset, view_width, view_height, draw_layer){
    this->type = UI_ELEMENT_TYPE::OBJECT_FRAME;

    this->reference = reference;
}

UIObjectFrame::~UIObjectFrame(){
}

void UIObjectFrame::draw(uint64_t delta, const SDL_Rect& draw_scope){
    SDL_Renderer* renderer = Engine::getInstance()->getRenderer();

    //Draw reference
    if(this->reference != nullptr &&
        SDL_RenderCopy(renderer, this->reference->getSprite()->texture, NULL, &draw_scope)){
        printf("%s\n", SDL_GetError());
    }
}

void UIObjectFrame::setReference(Object* reference){
    this->reference = reference;
}