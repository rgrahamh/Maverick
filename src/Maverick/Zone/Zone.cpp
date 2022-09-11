#include "./Zone.hpp"

Zone::Zone(const char* name, uint64_t global_x_coord, uint64_t global_y_coord){
    this->name = name;

    this->objects = new ObjectList;
    this->objects->obj = NULL;
    this->objects->next = NULL;

    this->ui = new UIElementList;
    this->ui->element = NULL;
    this->ui->next = NULL;

    this->global_x_coord = global_x_coord;
    this->global_y_coord = global_y_coord;
}

Zone::~Zone(){
    ObjectList* obj_cursor = objects;
    while(objects != NULL){
        obj_cursor = objects->next;
        if(objects->obj != nullptr){
            delete objects->obj;
        }
        delete objects;
        objects = obj_cursor;
    }

    UIElementList* ui_cursor = ui;
    while(ui != NULL){
        ui_cursor = ui->next;
        if(ui->element != nullptr){
            delete ui->element;
        }
        delete ui;
        ui = ui_cursor;
    }
}

ObjectList* Zone::getObjects(){
    return this->objects;
}

UIElementList* Zone::getUIElements(){
    return this->ui;
}

const char* Zone::getName(){
    return this->name;
}

uint64_t Zone::getGlobalX(){
    return this->global_x_coord;
}

uint64_t Zone::getGlobalY(){
    return this->global_y_coord;
}

void Zone::addObject(Object* object){
    if(object == nullptr){
        return;
    }
    if(this->objects != NULL && this->objects->obj == NULL){
        this->objects->obj = object;
    }
    else{
        ObjectList* new_lst = new ObjectList;
        new_lst->next = this->objects;
        new_lst->obj = object;
        this->objects = new_lst;
    }
}

void Zone::addUIElement(UIElement* element){
    if(this->ui != NULL && this->ui->element == NULL){
        this->ui->element = element;
    }
    else{
        UIElementList* new_lst = new UIElementList;
        new_lst->next = this->ui;
        new_lst->element = element;
        this->ui = new_lst;
    }
}