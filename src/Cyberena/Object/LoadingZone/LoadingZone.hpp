#ifndef LOADINGZONE_H
#define LOADINGZONE_H

#include "../Object.hpp"

class LoadingZone : public Object{
    public:
        LoadingZone(const char* name, const char* zone_to_load, float start_x, float start_y, float start_z, double width, double height, double depth, uint32_t matching_type, int layer = 1);
    private:
        char* zone_to_load;
        uint32_t matching_type;

		void onCollide(Object* other, Hitbox* this_hitbox, Hitbox* other_hitbox);
};

#endif