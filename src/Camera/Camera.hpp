#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Object/Object.hpp"

class Camera{
	public:
		Camera(Object* reference);
		~Camera();

		void setReference(Object* reference);
		void _draw(Object** obj_lst);

	private:
		Object* reference;
		float default_x;
		float default_y;
};

#endif