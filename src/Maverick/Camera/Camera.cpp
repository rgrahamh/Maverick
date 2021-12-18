#include "./Camera.hpp"

const float follow_rate = 0.08;

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
    SDL_GetRendererOutputSize(this->renderer, &win_width, &win_height);

    float obj_x = (reference->getX() + (reference->getWidth() / 2)) - ((win_width / 2) / x_scale);
    float obj_y = (reference->getY() + (reference->getHeight() / 2)) - ((win_height / 2) / y_scale);
    this->current_x = obj_x;
    this->current_y = obj_y;
    //this->current_x += ((obj_x - current_x) * follow_rate);
    //this->current_y += ((obj_y - current_y) * follow_rate);
}

/** Draws all objects in the given object list
 * @param obj_lst The object list that you wish to draw
 */
void Camera::_draw(ObjectList* obj_lst, uint32_t delta, double camera_x_offset, double camera_y_offset){
    recenter();

    while(obj_lst != NULL){
        if(obj_lst->obj->isVisible()){
            obj_lst->obj->_draw(renderer, delta, current_x + camera_x_offset, current_y + camera_y_offset);
        }
        obj_lst = obj_lst->next;
    }
}

/** Draws all UI elements in the given UI element list
 * @param obj_lst The UI element list that you wish to draw
 */
void Camera::_draw(UIElementList* element_lst, uint32_t delta){
    while(element_lst != NULL){
        if(element_lst->element->isVisible()){
            element_lst->element->_draw(renderer, delta, 0, 0);
        }
        element_lst = element_lst->next;
    }
}

SDL_Renderer* Camera::getRenderer(){
    return renderer;
}

/** Sets the X & Y scale of the camera
 * @param x_scale The X scale of the camera
 * @param x_scale The Y scale of the camera
 */
void Camera::setScale(float x_scale, float y_scale){
    this->x_scale = x_scale;
    this->y_scale = y_scale;
    SDL_RenderSetScale(this->renderer, x_scale, y_scale);
}