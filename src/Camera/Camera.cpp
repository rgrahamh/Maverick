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
void Camera::_draw(ObjectLst* obj_lst){
    SDL_RenderClear(renderer);

    recenter();

    ObjectLst* cursor = obj_lst;
    for(unsigned int i = 0; i < NUM_DRAW_LAYERS; i++){
        cursor = obj_lst;
        while(cursor != NULL){
            if(cursor->obj->getDrawLayer() == i && cursor->obj->isVisible()){
                cursor->obj->draw(renderer);
            }
            cursor = cursor->next;
        }
    }

    SDL_RenderPresent(renderer);
}