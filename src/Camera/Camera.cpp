#include "./Camera.hpp"

Camera::Camera(Object* reference){
    this->reference = reference;
    this->default_x = reference->getX();
    this->default_y = reference->getY();
}

void Camera::setReference(Object* reference){
    this->reference = reference;
}

void _draw(ObjLst* obj_lst){
    while(obj_lst != NULL){
        obj_lst->obj->_draw();
        obj_lst = obj_lst->next;
    }
}