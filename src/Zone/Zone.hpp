#ifndef ZONE_H
#define ZONE_H

#include "../Object/Object.hpp"

class Zone{
    public:
        Zone(const char* name);
        ~Zone();

        ObjectLst* getObjects();
        void addObject(Object* object);
        const char* getName();
    private:
        ObjectLst* objects;
        const char* name;
};

typedef struct ZoneList{
    Zone* zone;
    struct ZoneList* next;
} ZoneLst;

#endif