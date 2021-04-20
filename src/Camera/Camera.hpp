#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Object/Object.hpp"

#define NUM_DRAW_LAYERS 3

class Camera{
	public:
		Camera(sf::RenderWindow* window, Object* reference);
		~Camera();

		void setReference(Object* reference);
		void _draw(ObjectLst* obj_lst);
		void setWindow(sf::RenderWindow window);

	private:
		Object* reference;
		float default_x;
		float default_y;

		sf::RenderWindow* window;
		sf::View* view;

		void recenter();
};

#endif