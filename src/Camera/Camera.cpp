#include "./Camera.hpp"

Camera::Camera(){
}

Camera::Camera(sf::RenderWindow* window, Object* reference = NULL){
    this->reference = reference;
    this->default_x = reference->getX();
    this->default_y = reference->getY();
    this->view = new sf::View(sf::FloatRect(0.f, 0.f, (float)sf::VideoMode::getDesktopMode().width, (float)sf::VideoMode::getDesktopMode().height));
    this->window->setView(*this->view);
}

void Camera::setReference(Object* reference){
    this->reference = reference;
}

void Camera::recenter(){
    if(this->reference != NULL){
        this->view->setCenter(reference->getX() + (reference->getWidth() / 2), reference->getY() + (reference->getHeight() / 2));
    }
}

void Camera::_draw(ObjectLst* obj_lst){
    recenter();

    ObjectLst* ref = obj_lst;
    for(int i = 0; i < 3; i++){
        obj_lst = ref;
        while(obj_lst != NULL){
            if(obj_lst->obj->getDrawLayer() == i){
                obj_lst->obj->_draw(window, i);
            }
            obj_lst = obj_lst->next;
        }
    }
}