#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Object/Object.hpp"

#define NUM_DRAW_LAYERS 3
#define FOLLOW_RATE 0.08

class Camera{
	public:
		Camera(sf::RenderWindow* window, Object* reference);
		~Camera();

		void setReference(Object* reference);
		void _draw(ObjectLst* obj_lst);
		void setWindow(sf::RenderWindow window);

	private:
		Object* reference;
		float current_x;
		float current_y;

		sf::RenderWindow* window;
		sf::View* view;

		void recenter();
};

#endif