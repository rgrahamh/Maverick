#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Entity/Object/Object.hpp"
#include "../Entity/UI/UIElement.hpp"

#include <SDL2/SDL.h>

enum CAMERA_FOLLOW_MODE{
	FIXED_FOLLOW = 0,
	GRADUAL_FOLLOW = 1
};

class Camera{
	public:
		Camera(SDL_Renderer* renderer, SDL_Window* window, Object* reference, CAMERA_FOLLOW_MODE follow_mode, double follow_rate);

		void setReference(Object* reference);
		void _draw(ObjectList* obj_lst, uint64_t delta, double camera_x_offset = 0, double camera_y_offset = 0);
		void _draw(UIElementList* element_lst, uint64_t delta);

		SDL_Renderer* getRenderer();
		double getX();
		double getY();
		CAMERA_FOLLOW_MODE getFollowMode();
		double getFollowRate();

		void setScale(double x_scale, double y_scale);
		void setFollowMode(CAMERA_FOLLOW_MODE follow_mode);
		void setFollowRate(double follow_rate);

	private:
		Object* reference;
	    double current_x;
		double current_y;
		double x_scale;
		double y_scale;
		double follow_rate;

		CAMERA_FOLLOW_MODE follow_mode;

		SDL_Renderer* renderer;
		SDL_Window* window;

		void recenter();
};

#endif