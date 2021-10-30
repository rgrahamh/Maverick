#ifndef ZONE_H
#define ZONE_H

#include "../Entity/Object/Object.hpp"

class Zone{
    public:
        Zone(const char* name);
        ~Zone();

        ObjectList* getObjects();
        void addObject(Object* object);
        const char* getName();
    private:
        ObjectList* objects;
        const char* name;
};

struct ZoneList{
    Zone* zone;
    struct ZoneList* next;
};

#endif