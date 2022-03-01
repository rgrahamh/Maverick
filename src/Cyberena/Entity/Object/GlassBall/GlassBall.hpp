#ifndef GLASSBALL_H
#define GLASSBALL_H
#include "../Object.hpp"

class GlassBall : public Object{
    public:
        GlassBall(const char* name, float start_x, float start_y, float start_z, float friction, float mass, float terminal_velocity, bool gravity = true, int layer = 1);
    private:
		virtual void process(uint64_t delta, unsigned int steps);
};
#endif