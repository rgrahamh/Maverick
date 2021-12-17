#include "UIScreenBlit.hpp"
#include "../../../Engine/Engine.hpp"

extern Engine* engine;

/** Constructor for UIScreenBlit
 */
UIScreenBlit::UIScreenBlit(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                           unsigned int animation_num, int draw_layer, SDL_Window* window)
               : UIElement(name, view_x_offset, view_y_offset, view_width, view_height, animation_num, draw_layer, window)
{}

void UIScreenBlit::draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y){
    SDL_Texture* texture = engine->getScreenBlitTexture();
    if(texture != nullptr){
        SDL_RenderCopy(renderer, texture, nullptr, &this->draw_area);
    }
}