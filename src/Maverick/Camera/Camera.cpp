#include "./Camera.hpp"
#include "./../Engine/Engine.hpp"

/** The parameterized constructor for the camera
 * @param window The window that the camera should be drawing to
 * @param reference The object that the camera is centered on
 */
Camera::Camera(SDL_Renderer* renderer, SDL_Window* window, Object* reference = nullptr, CAMERA_FOLLOW_MODE follow_mode = CAMERA_FOLLOW_MODE::FIXED_FOLLOW, double follow_rate = 0.08){
    this->reference = reference;
    this->current_x = 0;
    this->current_y = 0;
    if(reference != NULL){
        this->current_x = reference->getX();
        this->current_y = reference->getY();
    }
    this->window = window;
    this->renderer = renderer;
    this->follow_mode = follow_mode;
    this->follow_rate = follow_rate;

    this->x_scale = 1.0;
    this->y_scale = 1.0;
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

    int win_x, win_y;
    SDL_GetRendererOutputSize(this->renderer, &win_x, &win_y);
    float obj_x = (reference->getX() + (reference->getWidth() / 2)) - ((win_x / 2) / x_scale);
    float obj_y = (reference->getY() + (reference->getHeight() / 2)) - ((win_y / 2) / y_scale);
    if(follow_mode == CAMERA_FOLLOW_MODE::FIXED_FOLLOW){
        this->current_x = obj_x;
        this->current_y = obj_y;
    }
    else{
        this->current_x += ((obj_x - current_x) * follow_rate);
        this->current_y += ((obj_y - current_y) * follow_rate);
    }
}

/** Draws all objects in the given object list
 * @param obj_lst The object list that you wish to draw
 */
void Camera::_draw(ObjectList* obj_lst, uint64_t delta, double camera_x_offset, double camera_y_offset){
    recenter();

    int win_width, win_height;
    SDL_GetRendererOutputSize(this->renderer, &win_width, &win_height);

    while(obj_lst != NULL){
        //If visible and on-screen
        if(obj_lst->obj->isVisible() &&
           !(obj_lst->obj->getX() > this->current_x + win_width || obj_lst->obj->getX() + obj_lst->obj->getWidth() < this->current_x) &&
           !(obj_lst->obj->getY() - obj_lst->obj->getZ() > this->current_y + win_height || obj_lst->obj->getY() - obj_lst->obj->getZ() + obj_lst->obj->getHeight() < this->current_y)){
            obj_lst->obj->_draw(renderer, delta, current_x + camera_x_offset, current_y + camera_y_offset);
        }
        obj_lst = obj_lst->next;
    }
}

/** Draws all UI elements in the given UI element list
 * @param obj_lst The UI element list that you wish to draw
 */
void Camera::_draw(UIElementList* element_lst, uint64_t delta){
    while(element_lst != NULL){
        if(element_lst->element->isVisible()){
            element_lst->element->_draw(renderer, delta, 0, 0);
        }
        element_lst = element_lst->next;
    }
}

/** Gets the renderer the camera's using
 * @return The renderer the camera's using
 */
SDL_Renderer* Camera::getRenderer(){
    return renderer;
}

/** Gets the X coord of the top-left corner of the camera
 * @return The X coord of the top-left corner of the camera
 */
double Camera::getX(){
    return this->current_x;
}

/** Gets the Y coord of the top-left corner of the camera
 * @return The Y coord of the top-left corner of the camera
 */
double Camera::getY(){
    return this->current_y;
}

/** Gets the follow mode of the camera
 * @return The follow mode of the camera
 */
CAMERA_FOLLOW_MODE Camera::getFollowMode(){
    return this->follow_mode;
}

/** Gets the follow rate of the camera
 * @return The follow rate of the camera
 */
double Camera::getFollowRate(){
    return this->follow_rate;
}

/** Sets the X & Y scale of the camera
 * @param x_scale The X scale of the camera
 * @param y_scale The Y scale of the camera
 */
void Camera::setScale(double x_scale, double y_scale){
    this->x_scale = x_scale;
    this->y_scale = y_scale;
    SDL_RenderSetScale(this->renderer, x_scale, y_scale);
}

/** Sets the camera follow mode
 * @param follow_mode The new follow mode of the camera
 */
void Camera::setFollowMode(CAMERA_FOLLOW_MODE follow_mode){
    this->follow_mode = follow_mode;
}

/** Sets the camera follow rate
 * @param follow_rate The new follow rate of the camera
 */
void Camera::setFollowRate(double follow_rate){
    this->follow_rate = follow_rate;
}