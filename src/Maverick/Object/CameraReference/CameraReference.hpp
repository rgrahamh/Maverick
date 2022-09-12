#ifndef CAMERAREFERENCE_H
#define CAMERAREFERENCE_H
#include "../Object.hpp"

class CameraReference : public Object{
    public:
		CameraReference(const char* name, float start_x, float start_y, double x_speed = 1, double y_speed = 1);
		void action(Control* control);
    void process(uint64_t delta, unsigned int steps);

    private:
        double x_speed;
        double y_speed;
};

#endif