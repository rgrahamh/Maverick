#ifndef OBJECT_H
#define OBJECT_H

#include "../../Animation/Animation.hpp"
#include "../../Animation/Hitbox/Hitbox.hpp"
#include "../../Animation/Hitbox/HitRect/HitRect.hpp"
#include "../../Animation/Hitbox/HitEllipse/HitEllipse.hpp"
#include "../../Animation/Hitbox/HitEllipse/HitCone/HitCone.hpp"
#include "../../Animation/Hitbox/HitboxCollision.hpp"
#include "../../Control/Control.hpp"
#include "../Entity.hpp"

#include <SDL2/SDL.h>
#include <unordered_map>

class Object : public Entity{
	public:
		Object(const char* name, float start_x, float start_y, float friction, float mass, unsigned int animation_num, int draw_layer = 1);
		virtual ~Object();
		float getOldX();
		float getOldY();
		float getXVel();
		float getYVel();
		float getMass();
		bool getEnvBump();
		
		void setXVel(float xV);
		void setYVel(float yV);
		void setEnvBump();

		void applyForce(float xA, float yA);

		//Processing functions
		virtual void _process(uint32_t delta);
		//Need this for custom processing
		virtual void process(uint32_t delta);

		virtual void _action(Control* control);
		virtual void action(Control* control);

		virtual void _draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y);
		virtual void draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y);

		virtual void onCollide(Object* other, Hitbox* this_hitbox, Hitbox* other_hitbox);
		
	protected:
		//Previous position (used for rollback)
		float old_x;
		float old_y;

		//Velocity
		float xV;
		float yV;

		//Acceleration
		float xA;
		float yA;

		//Coefficient of friction
		float friction;

		//How much the object weighs
		float mass;

		//Environmental bump
		bool env_bump;
};

struct ObjectList{
	Object* obj;
	struct ObjectList* next;
};

#endif