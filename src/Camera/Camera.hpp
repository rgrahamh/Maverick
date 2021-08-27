#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Object/Object.hpp"

#include <SDL2/SDL.h>

#define NUM_DRAW_LAYERS 3
#define FOLLOW_RATE 0.08

class Camera{
	public:
		Camera(SDL_Renderer* renderer, Object* reference);
		~Camera();

		void setReference(Object* reference);
		void _draw(ObjectLst* obj_lst);
		void setRenderer(SDL_Renderer* renderer);

	private:
		Object* reference;
		float current_x;
		float current_y;

		SDL_Renderer* renderer;
		sf::View* view;

		void recenter();
};

#endif