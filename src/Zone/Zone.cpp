#include "./Zone.hpp"

Zone::Zone(char* name){
    this->name = name;
    this->objects = new ObjectLst();
    this->objects->obj = NULL;
    this->objects->next = NULL;
}

ObjectLst* Zone::getObjects(){
    return this->objects;
}

char* Zone::getName(){
    return this->name;
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