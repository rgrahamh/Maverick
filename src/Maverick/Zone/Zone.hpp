#ifndef ZONE_H
#define ZONE_H

#include "../Entity/Object/Object.hpp"
#include "../Entity/UI/UIElement.hpp"
#include "../Filter/Filter.hpp"

class Zone{
    public:
        Zone(const char* name, uint64_t global_x_coord, uint64_t global_y_coord);
        ~Zone();

        ObjectList* getObjects();
        UIElementList* getUIElements();
        FilterList* getFilters();
        uint64_t getGlobalX();
        uint64_t getGlobalY();

        void addObject(Object* object);
        void addUIElement(UIElement* element);
        void addFilter(Filter* filter);
        const char* getName();
    private:
        ObjectList* objects;
        UIElementList* ui;
        FilterList* filters;
        const char* name;

        uint64_t global_x_coord;
        uint64_t global_y_coord;
};

struct ZoneList{
    Zone* zone;
    struct ZoneList* next;
};

#endif