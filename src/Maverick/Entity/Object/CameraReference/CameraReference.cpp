#include "./CameraReference.hpp"
#include "../../../Engine/Engine.hpp"
extern Engine* engine;

CameraReference::CameraReference(const char* name, float start_x, float start_y, float x_speed, float y_speed)
               : Object(name, start_x, start_y, 1, 0, 0){
    this->x_speed = x_speed;
    this->y_speed = y_speed;
}

void CameraReference::action(Control* control){
    const uint8_t* keys = control->getKeys();
    if(keys[SDL_SCANCODE_W] | keys[SDL_SCANCODE_UP]){
        this->yA -= y_speed / engine->getGlobalYScale();
    }
    if(keys[SDL_SCANCODE_A] | keys[SDL_SCANCODE_LEFT]){
        this->xA -= x_speed / engine->getGlobalXScale();
    }
    if(keys[SDL_SCANCODE_S] | keys[SDL_SCANCODE_DOWN]){
        this->yA += y_speed / engine->getGlobalYScale();
    }
    if(keys[SDL_SCANCODE_D] | keys[SDL_SCANCODE_RIGHT]){
        this->xA += x_speed / engine->getGlobalXScale();
    }
}