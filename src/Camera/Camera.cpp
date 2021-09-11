#include "./Camera.hpp"

/** The parameterized constructor for the camera
 * @param window The window that the camera should be drawing to
 * @param reference The object that the camera is centered on
 */
Camera::Camera(SDL_Renderer* renderer, Object* reference = NULL){
    this->reference = reference;
    if(reference != NULL){
        this->current_x = reference->getX();
        this->current_y = reference->getY();
    }
    this->renderer = renderer;
    this->viewport = new SDL_Rect();
    this->viewport->x = 0;
    this->viewport->y = 0;
    this->viewport->w = 1920;
    this->viewport->y = 1080;
}

Camera::~Camera(){
}

/** Sets the reference for the center of the camera
 * @param reference The reference object you wish to center on
 */
void Camera::setReference(Object* reference){
    this->reference = reference;
}

/** Recenters the camera on the reference object
 */
void Camera::recenter(){
    /*if(this->reference != NULL){
        float obj_x = reference->getX() + (reference->getWidth() / 2);
        float obj_y = reference->getY() + (reference->getHeight() / 2);
        current_x += (obj_x - current_x) * FOLLOW_RATE;
        current_y += (obj_y - current_y) * FOLLOW_RATE;
    } Figure out how to recenter later*/
}

/** Draws all objects in the given object list
 * @param obj_lst The object list that you wish to draw
 */
void Camera::_draw(ObjectLst* obj_lst, uint32_t delta){
    recenter();

    SDL_RenderClear(renderer);

    /*if(SDL_RenderSetViewport(renderer, this->viewport)){
        printf("Cannot set viewport: %s\n", SDL_GetError());
    }*/

    while(obj_lst != NULL){
        if(obj_lst->obj->isVisible()){
            obj_lst->obj->draw(renderer, delta);
        }
        obj_lst = obj_lst->next;
    }

    SDL_RenderPresent(renderer);
}