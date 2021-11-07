#ifndef ZONE_H
#define ZONE_H

#include "../Entity/Object/Object.hpp"
#include "../Entity/UI/UIElement.hpp"

class Zone{
    public:
        Zone(const char* name);
        ~Zone();

        ObjectList* getObjects();
        UIElementList* getUIElements();

        void addObject(Object* object);
        void addUIElement(UIElement* element);
        const char* getName();
    private:
        ObjectList* objects;
        UIElementList* ui;
        const char* name;
};

struct ZoneList{
    Zone* zone;
    struct ZoneList* next;
};

#endif