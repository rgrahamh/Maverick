#include "./Camera.hpp"

/** The default constructor for the camera
 */
Camera::Camera(){
}

/** The parameterized constructor for the camera
 * @param window The window that the camera should be drawing to
 * @param reference The object that the camera is centered on
 */
Camera::Camera(sf::RenderWindow* window, Object* reference = NULL){
    this->reference = reference;
    this->default_x = reference->getX();
    this->default_y = reference->getY();
    this->view = new sf::View(sf::FloatRect(0.f, 0.f, (float)sf::VideoMode::getDesktopMode().width, (float)sf::VideoMode::getDesktopMode().height));
    this->window->setView(*this->view);
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
    recenter();

    ObjectLst* ref = obj_lst;
    for(int i = 0; i < 3; i++){
        obj_lst = ref;
        while(obj_lst != NULL){
            if(obj_lst->obj->getDrawLayer() == i){
                obj_lst->obj->_draw(window);
            }
            obj_lst = obj_lst->next;
        }
    }
}