#include "./Zone.hpp"

Zone::Zone(const char* name){
    this->name = name;
    this->objects = new ObjectList;
    this->objects->obj = NULL;
    this->objects->next = NULL;
}

Zone::~Zone(){
    ObjectList* obj_cursor = objects;
    while(objects != NULL){
        obj_cursor = objects->next;
        delete objects->obj;
        delete objects;
        objects = obj_cursor;
    }
}

ObjectList* Zone::getObjects(){
    return this->objects;
}

const char* Zone::getName(){
    return this->name;
}

void Zone::addObject(Object* object){
    if(this->objects->obj == NULL){
        this->objects->obj = object;
    }
    else{
        ObjectList* new_lst = new ObjectList;
        new_lst->next = this->objects;
        new_lst->obj = object;
        this->objects = new_lst;
    }
}