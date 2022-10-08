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
		Camera();

		/** Sets the reference for the center of the camera
		 * @param reference The reference object you wish to center on
		 */
		void setReference(Object* reference);

		/** Sets the camera zoom
		 * @param zoom The new camera zoom
		 */
		void setZoom(double zoom);

		/** Reset the zoom back to native
		 */
		void resetZoom();

		/** Draws all objects in the given object list
		 * @param obj_lst The object list that you wish to draw
		 */
		void _draw(ObjectList* obj_lst, uint64_t delta);

		/** Draws all UI elements in the given UI element list
		 * @param obj_lst The UI element list that you wish to draw
		 */
		void _draw(UIElementList* element_lst, uint64_t delta);

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

		/** Gets the camera zoom
		 * @return The current camera zoom
		 */
		double getZoom();

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
		double follow_rate;

		double zoom;

		CAMERA_FOLLOW_MODE follow_mode;

		/** Recenters the camera on the reference object
		 * @param delta The time that has passed since the last recenter
		 */
		void recenter(uint64_t delta);
};

#endif