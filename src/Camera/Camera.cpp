#include "./Camera.hpp"

/** The parameterized constructor for the camera
 * @param window The window that the camera should be drawing to
 * @param reference The object that the camera is centered on
 */
Camera::Camera(sf::RenderWindow* window, Object* reference = NULL){
    this->reference = reference;
    if(reference != NULL){
        this->default_x = reference->getX();
        this->default_y = reference->getY();
    }
    this->view = new sf::View(window->getDefaultView());
    this->window = window;
    this->window->setView(*(this->view));
}

Camera::~Camera(){
    this->window->setView(this->window->getDefaultView());
    delete this->view;
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
    if(this->reference != NULL){
        this->view->setCenter(reference->getX() + (reference->getWidth() / 2), reference->getY() + (reference->getHeight() / 2));
    }
}

/** Draws all objects in the given object list
 * @param obj_lst The object list that you wish to draw
 */
void Camera::_draw(ObjectLst* obj_lst){
    window->clear();

    recenter();

    ObjectLst* cursor = obj_lst;
    for(unsigned int i = 0; i < 3; i++){
        cursor = obj_lst;
        while(cursor != NULL){
            if(cursor->obj->getDrawLayer() == i){
                cursor->obj->draw(window);
            }
            cursor = cursor->next;
        }
    }

    window->display();
}