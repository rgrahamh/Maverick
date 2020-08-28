#ifndef ZONE_H
#define ZONE_H

#include "../Object/Object.hpp"

typedef struct ZoneList{
    Zone* zone;
    ZoneLst* next;
} ZoneLst;

class Zone{
    public:
        Zone();
        ~Zone();

        ObjLst* getObjects();
    private:
        ObjLst* objects;
};

#endif