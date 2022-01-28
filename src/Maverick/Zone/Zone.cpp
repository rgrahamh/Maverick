#include "./Zone.hpp"

/** The zone constructor
 * @param name The name of the zone
 */
Zone::Zone(const char* name, uint64_t global_x_coord, uint64_t global_y_coord){
    this->name = name;

    this->objects = new ObjectList;
    this->objects->obj = NULL;
    this->objects->next = NULL;

    this->ui = new UIElementList;
    this->ui->element = NULL;
    this->ui->next = NULL;

    this->filters = new FilterList;
    this->filters->filter = nullptr;
    this->filters->next = nullptr;

    this->global_x_coord = global_x_coord;
    this->global_y_coord = global_y_coord;
}

/** The zone default destructor
 */
Zone::~Zone(){
    ObjectList* obj_cursor = objects;
    while(objects != NULL){
        obj_cursor = objects->next;
        delete objects->obj;
        delete objects;
        objects = obj_cursor;
    }

    UIElementList* ui_cursor = ui;
    while(ui != NULL){
        ui_cursor = ui->next;
        delete ui->element;
        delete ui;
        ui = ui_cursor;
    }
}

/** Gets the all objects in the zone
 * @return An ObjectList* of all objects in the zone
 */
ObjectList* Zone::getObjects(){
    return this->objects;
}

/** Gets the all objects in the zone
 * @return An ObjectList* of all objects in the zone
 */
UIElementList* Zone::getUIElements(){
    return this->ui;
}

/** Gets the name of the zone
 * @return The name of the zone
 */
const char* Zone::getName(){
    return this->name;
}

/** Gets the global X coordinate
 * @return The global X coodrdinate
 */
uint64_t Zone::getGlobalX(){
    return this->global_x_coord;
}

/** Gets the global Y coordinate
 * @return The global Y coodrdinate
 */
uint64_t Zone::getGlobalY(){
    return this->global_y_coord;
}

/** Adds an object to the zone
 * @param element The object to add to the zone
 */
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

/** Adds a UI element to the zone
 * @param element The element to add to the zone
 */
void Zone::addUIElement(UIElement* element){
    if(this->ui->element == NULL){
        this->ui->element = element;
    }
    else{
        UIElementList* new_lst = new UIElementList;
        new_lst->next = this->ui;
        new_lst->element = element;
        this->ui = new_lst;
    }
}

void Zone::addFilter(Filter* filter){
    if(this->filters->filter == nullptr){
        this->filters->filter = filter;
    }
    else{
        FilterList* new_lst = new FilterList;
        new_lst->next = this->filters;
        new_lst->filter = filter;
        this->filters = new_lst; 
    }
}