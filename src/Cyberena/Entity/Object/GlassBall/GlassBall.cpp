#include "GlassBall.hpp"
GlassBall::GlassBall(const char* name, float start_x, float start_y, float start_z, float friction, float mass, float terminal_velocity, bool gravity, int layer)
         : Object(name, start_x, start_y, start_z, friction, mass, terminal_velocity, gravity, layer)
         {
             this->type = EXTENDED_OBJECT_TYPE::BALL;
         };

void GlassBall::process(uint64_t delta, unsigned int steps){
    if(abs(this->xV) + abs(this->yV) < 0.01){
        this->xV = 0;
        this->yV = 0;
        this->setAnimation("neutral");
    }
    else{
        this->setAnimation("rolling");
    }
}