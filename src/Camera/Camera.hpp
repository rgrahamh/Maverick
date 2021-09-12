#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Object/Object.hpp"

#include <SDL2/SDL.h>

#define MIN_DRAW_LAYER -3
#define MAX_DRAW_LAYER 3
#define FOLLOW_RATE 0.08

class Camera{
	public:
		Camera(SDL_Renderer* renderer, Object* reference);
		~Camera();

		void setReference(Object* reference);
		void _draw(ObjectLst* obj_lst, uint32_t delta);
		SDL_Renderer* getRenderer();

	private:
		Object* reference;
		float current_x;
		float current_y;

		SDL_Renderer* renderer;
		SDL_Rect* viewport;

		void recenter();
};

#endif