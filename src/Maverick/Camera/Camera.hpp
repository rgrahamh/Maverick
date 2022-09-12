#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../Object/Object.hpp"
#include "../UIElement/UIElement.hpp"

#include <SDL2/SDL.h>

enum CAMERA_FOLLOW_MODE{
	FIXED_FOLLOW = 0,
	GRADUAL_FOLLOW = 1
};

class Camera{
	public:
		/** The parameterized constructor for the camera
		 * @param window The window that the camera should be drawing to
		 * @param reference The object that the camera is centered on
		 */
		Camera(SDL_Renderer* renderer, SDL_Window* window, Object* reference, CAMERA_FOLLOW_MODE follow_mode, double follow_rate);

		/** Sets the reference for the center of the camera
		 * @param reference The reference object you wish to center on
		 */
		void setReference(Object* reference);

		/** Draws all objects in the given object list
		 * @param obj_lst The object list that you wish to draw
		 */
		void _draw(ObjectList* obj_lst, uint64_t delta, double camera_x_offset = 0, double camera_y_offset = 0);

		/** Draws all UI elements in the given UI element list
		 * @param obj_lst The UI element list that you wish to draw
		 */
		void _draw(UIElementList* element_lst, uint64_t delta);

		/** Gets the renderer the camera's using
		 * @return The renderer the camera's using
		 */
		SDL_Renderer* getRenderer();

		/** Gets the X coord of the top-left corner of the camera
		 * @return The X coord of the top-left corner of the camera
		 */
		double getX();

		/** Gets the Y coord of the top-left corner of the camera
		 * @return The Y coord of the top-left corner of the camera
		 */
		double getY();

		/** Gets the follow mode of the camera
		 * @return The follow mode of the camera
		 */
		CAMERA_FOLLOW_MODE getFollowMode();

		/** Gets the follow rate of the camera
		 * @return The follow rate of the camera
		 */
		double getFollowRate();

		/** Sets the X & Y scale of the camera
		 * @param x_scale The X scale of the camera
		 * @param y_scale The Y scale of the camera
		 */
		void setScale(double x_scale, double y_scale);

		/** Sets the camera follow mode
		 * @param follow_mode The new follow mode of the camera
		 */
		void setFollowMode(CAMERA_FOLLOW_MODE follow_mode);

		/** Sets the camera follow rate
		 * @param follow_rate The new follow rate of the camera
		 */
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

		/** Recenters the camera on the reference object
		 */
		void recenter();
};

#endif