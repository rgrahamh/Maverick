#include "./Camera.hpp"
#include "Maverick/Engine/Engine.hpp"

Camera::Camera(){
    this->reference = nullptr;
    this->follow_mode = CAMERA_FOLLOW_MODE::FIXED_FOLLOW;
    this->follow_rate = 0.08;
    this->current_x = 0;
    this->current_y = 0;
    if(reference != NULL){
        this->current_x = reference->getX();
        this->current_y = reference->getY();
    }

    this->zoom = 1.0;
}

void Camera::setReference(Object* reference){
    this->reference = reference;
}

void Camera::setZoom(double zoom){
    this->zoom = zoom;
    SDL_RenderSetScale(Engine::getInstance()->getRenderer(), zoom, zoom);
}

void Camera::resetZoom(){
    zoom = 1.0;
    SDL_RenderSetScale(Engine::getInstance()->getRenderer(), 1.0, 1.0);
}

void Camera::recenter(){
    if(this->reference == NULL){
        return;
    }

    int win_width, win_height;
    SDL_GetRendererOutputSize(Engine::getInstance()->getRenderer(), &win_width, &win_height);
    float obj_x = (reference->getX() + (reference->getWidth() / 2)) - ((win_width / 2) / zoom);
    float obj_y = (reference->getY() + (reference->getHeight() / 2)) - ((win_height / 2) / zoom);
    if(follow_mode == CAMERA_FOLLOW_MODE::FIXED_FOLLOW){
        this->current_x = obj_x;
        this->current_y = obj_y;
    }
    else{
        this->current_x += ((obj_x - current_x) * follow_rate);
        this->current_y += ((obj_y - current_y) * follow_rate);
    }
}

void Camera::_draw(ObjectList* obj_lst, uint64_t delta){
    SDL_Renderer* renderer = Engine::getInstance()->getRenderer();

    recenter();

    int win_width, win_height;
    SDL_GetRendererOutputSize(renderer, &win_width, &win_height);

    while(obj_lst != NULL){
        //If visible and on-screen
        if(obj_lst->obj->isVisible() &&
           !(obj_lst->obj->getX() > this->current_x + win_width || obj_lst->obj->getX() + obj_lst->obj->getWidth() < this->current_x) &&
           !(obj_lst->obj->getY() - obj_lst->obj->getZ() > this->current_y + win_height || obj_lst->obj->getY() - obj_lst->obj->getZ() + obj_lst->obj->getHeight() < this->current_y)){
            obj_lst->obj->_draw(delta, current_x, current_y);
        }
        obj_lst = obj_lst->next;
    }
}

void Camera::_draw(UIElementList* element_lst, uint64_t delta){
    SDL_Renderer* renderer = Engine::getInstance()->getRenderer();
    SDL_Rect screen_area;
    screen_area.x = 0;
    screen_area.y = 0;
    SDL_GetRendererOutputSize(renderer, &screen_area.w, &screen_area.h);

    while(element_lst != NULL){
        if(element_lst->element->isVisible()){
            element_lst->element->_draw(delta, screen_area);
        }
        element_lst = element_lst->next;
    }
}

double Camera::getX(){
    return this->current_x;
}

double Camera::getY(){
    return this->current_y;
}

CAMERA_FOLLOW_MODE Camera::getFollowMode(){
    return this->follow_mode;
}

double Camera::getFollowRate(){
    return this->follow_rate;
}

double Camera::getZoom(){
    return this->zoom;
}

void Camera::setFollowMode(CAMERA_FOLLOW_MODE follow_mode){
    this->follow_mode = follow_mode;
}

void Camera::setFollowRate(double follow_rate){
    this->follow_rate = follow_rate;
}