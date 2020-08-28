#include "./Engine.hpp"

Engine::Engine(ZoneLst* zones){
    this->zones = zones;
    Camera* camera = new Camera();
}

Engine::~Engine(){
    delete camera;
}

void Engine::draw(){
    //Make a combined deep copy of the object list (so that things of different zones can interact if they're adjacent)
    ObjLst* all_objects = new ObjLst;
    ObjLst* obj_iter = all_objects;
    while(this->active_zones != NULL){
        ObjLst* new_objects = active_zones->zone->getObjects();
        while(new_objects != NULL){
            obj_iter->next = new ObjLst;
            obj_iter = obj_iter->next;
            obj_iter->obj = new_objects->obj;
            new_objects = new_objects->next;
        }
    }
    obj_iter->next = NULL;

    //Bubble sort by height (since things aren't typically changing order that much between frames). That way we don't have to compare objects outside of its height scope.
    bool done = false;
    while(!done){
        obj_iter = all_objects;
        done = true;
        while(obj_iter != NULL){
            if ((obj_iter->obj->getY() + obj_iter->obj->getHeight()) > (obj_iter->next->obj->getY() + obj_iter->next->obj->getHeight())){
                done = false;
                Object* tmp = obj_iter->next->obj;
                obj_iter->next->obj = obj_iter->obj;
                obj_iter->obj = tmp;
            }
            obj_iter = obj_iter->next;
        }
    }
    camera->_draw(active_zones->zone->getObjects());

    //TODO: DON'T FORGET TO WRITE CODE TO FREE THE MEMORY USED FOR THE NEW ALL_OBJECTS!!!
}