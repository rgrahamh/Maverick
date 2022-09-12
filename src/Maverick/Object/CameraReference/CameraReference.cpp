#include "./CameraReference.hpp"
#include "Maverick/Engine/Engine.hpp"

CameraReference::CameraReference(const char* name, float start_x, float start_y, double x_speed, double y_speed)
               : Object(name, start_x, start_y, 0, 0, 0, 4){
    this->type = OBJECT_TYPE::CAMERA_REFERENCE;

    this->x_speed = x_speed;
    this->y_speed = y_speed;
}

void CameraReference::action(Control* control){
    Engine* engine = Engine::getInstance();
    const uint8_t* keys = control->getKeys();
    if(!(engine->getState() & GAME_STATE::PAUSE)){
        if(keys[SDL_SCANCODE_W] | keys[SDL_SCANCODE_UP]){
            this->y -= y_speed / engine->getGlobalYScale();
        }
        if(keys[SDL_SCANCODE_A] | keys[SDL_SCANCODE_LEFT]){
            this->x -= x_speed / engine->getGlobalXScale();
        }
        if(keys[SDL_SCANCODE_S] | keys[SDL_SCANCODE_DOWN]){
            this->y += y_speed / engine->getGlobalYScale();
        }
        if(keys[SDL_SCANCODE_D] | keys[SDL_SCANCODE_RIGHT]){
            this->x += x_speed / engine->getGlobalXScale();
        }

        engine->getCamera()->setFollowMode(CAMERA_FOLLOW_MODE::GRADUAL_FOLLOW);
    }
}

void CameraReference::process(uint64_t delta, unsigned int steps){
    this->xV = this->x_speed;
    this->yV = this->y_speed;
}