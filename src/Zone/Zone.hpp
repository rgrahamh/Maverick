#ifndef ZONE_H
#define ZONE_H

class Zone{
    public:
        Zone();
        ~Zone();
    private:
        Object** objects;
};

#endif