#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Entity/Object/Object.hpp"
#include "../Entity/UI/UIElement.hpp"

#include <SDL2/SDL.h>

#define FOLLOW_RATE 0.04

class Camera{
	public:
		Camera(SDL_Renderer* renderer, SDL_Window* window, Object* reference);
		~Camera();

		void setReference(Object* reference);
		void _draw(ObjectList* obj_lst, uint32_t delta);
		void _draw(UIElementList* element_lst, uint32_t delta);
		SDL_Renderer* getRenderer();

	private:
		Object* reference;
	    float current_x;
		float current_y;

		SDL_Renderer* renderer;
		SDL_Window* window;

		void recenter();
};

#endif