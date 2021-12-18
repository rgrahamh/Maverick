#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Entity/Object/Object.hpp"
#include "../Entity/UI/UIElement.hpp"

#include <SDL2/SDL.h>

class Camera{
	public:
		Camera(SDL_Renderer* renderer, SDL_Window* window, Object* reference);
		~Camera();

		void setReference(Object* reference);
		void _draw(ObjectList* obj_lst, uint32_t delta, double camera_x_offset = 0, double camera_y_offset = 0);
		void _draw(UIElementList* element_lst, uint32_t delta);
		SDL_Renderer* getRenderer();

		void setScale(float x_scale, float y_scale);

	private:
		Object* reference;
	    double current_x;
		double current_y;
		float x_scale;
		float y_scale;

		SDL_Renderer* renderer;
		SDL_Window* window;

		void recenter();
};

#endif