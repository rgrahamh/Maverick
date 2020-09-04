#ifndef ZONE_H
#define ZONE_H

#include "../Object/Object.hpp"

class Zone{
    public:
        Zone(char* name);
        ~Zone();

        ObjectLst* getObjects();
        void addObject(Object* object);
        char* getName();
    private:
        ObjectLst* objects;
        char* name;
};

typedef struct ZoneList{
    Zone* zone;
    struct ZoneList* next;
} ZoneLst;

#endif