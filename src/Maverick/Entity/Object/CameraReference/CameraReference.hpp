#ifndef CAMERAREFERENCE_H
#define CAMERAREFERENCE_H
#include "../Object.hpp"

class CameraReference : public Object{
    public:
		CameraReference(const char* name, float start_x, float start_y, float x_speed = 1, float y_speed = 1);
		void action(Control* control);

    private:
        float x_speed;
        float y_speed;
};

#endif