#include "./Zone.hpp"

Zone::Zone(){
    this->objects = new ObjectLst();
    this->objects->obj = NULL;
    this->objects->next = NULL;
}

ObjectLst* Zone::getObjects(){
    return this->objects;
}

void Zone::addObject(Object* object){
    if(this->objects->obj != NULL){
        this->objects->obj = object;
    }
    else{
        ObjectLst* new_lst = new ObjectLst;
        new_lst->next = this->objects;
        new_lst->obj = object;
        this->objects = new_lst;
    }
}