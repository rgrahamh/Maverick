#include "./Camera.hpp"

/** The parameterized constructor for the camera
 * @param window The window that the camera should be drawing to
 * @param reference The object that the camera is centered on
 */
Camera::Camera(SDL_Renderer* renderer, SDL_Window* window, Object* reference = NULL){
    this->reference = reference;
    this->current_x = 0;
    this->current_y = 0;
    if(reference != NULL){
        this->current_x = reference->getX();
        this->current_y = reference->getY();
    }
    this->window = window;
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
    if(this->reference == NULL){
        return;
    } 

    int win_width, win_height;
    SDL_GetWindowSize(this->window, &win_width, &win_height);

    float obj_x = reference->getX() + (reference->getWidth() / 2) - (win_width / 2);
    float obj_y = reference->getY() + (reference->getHeight() / 2) - (win_height / 2);
    current_x += (obj_x - current_x) * FOLLOW_RATE;
    current_y += (obj_y - current_y) * FOLLOW_RATE;
}

/** Draws all objects in the given object list
 * @param obj_lst The object list that you wish to draw
 */
void Camera::_draw(ObjectList* obj_lst, uint32_t delta){
    recenter();

    while(obj_lst != NULL){
        if(obj_lst->obj->isVisible()){
            obj_lst->obj->_draw(renderer, delta, current_x, current_y);
        }
        obj_lst = obj_lst->next;
    }
}

SDL_Renderer* Camera::getRenderer(){
    return renderer;
}