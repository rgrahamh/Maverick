#ifndef OBJECT_H
#define OBJECT_H

#include "../Animation/Animation.hpp"
#include "../Animation/Hitbox/Hitbox.hpp"
#include "../Animation/Hitbox/HitRect/HitRect.hpp"
#include "../Animation/Hitbox/HitEllipse/HitEllipse.hpp"
#include "../Animation/Hitbox/HitEllipse/HitCone/HitCone.hpp"
#include "../Animation/Hitbox/HitboxCollision.hpp"

#include <SDL2/SDL.h>

class Object{
	public:
		Object(float start_x, float start_y, float friction, float weight, unsigned int animation_num, bool animated = true);
		virtual ~Object();
		float getX();
		float getY();
		float getOldX();
		float getOldY();
		float getXVel();
		float getYVel();
		float getWidth();
		float getHeight();
		float getMass();
		unsigned int getDrawLayer();
		float getDrawAxis();
		HitboxLst* getHitboxes();
		bool isActive();
		bool isVisible();
		bool getEnvBump();

		void addSprite(unsigned int animation_num, const char* sprite_path, unsigned int keyframe, float x_offset, float y_offset);

		void addHitbox(unsigned int animation_num, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type, int sprite_num);
		void addHitbox(unsigned int animation_start, unsigned int animation_end, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type);
		void addHitbox(unsigned int animation_num, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type);
		void addHitbox(unsigned int animation_num, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type, float angle, float slice_prop, int sprite_num);
		void addHitbox(unsigned int animation_start, unsigned int animation_end, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type, float angle, float slice_prop);
		void addHitbox(unsigned int animation_num, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type, float angle, float slice_prop);

		void setX(float x);
		void setY(float y);
		void setXVel(float xV);
		void setYVel(float yV);
		void setAnimation(unsigned int animation_num);
		void setScale(unsigned int animation_num, float x_scale, float y_scale);
		void setScale(float x_scale, float y_scale);
		void setActive(bool active);
		void setVisible(bool active);
		void setEnvBump();

		void applyForce(float xA, float yA);

		//Processing functions
		void _process(uint32_t delta);
		//Need this for custom processing (like input)
		virtual void process();

		//Useful for actions on other objects & input
		virtual void action(SDL_Event* event);

		virtual void draw(SDL_Renderer* renderer);

		virtual void onCollide(Object* other, Hitbox* this_hitbox, Hitbox* other_hitbox);
		
	protected:
		//Position
		float x;
		float y;

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

		//If the object is active/visible
		bool active;
		bool visible;

		//Environmental bump
		bool env_bump;

		//Animation tracking
		unsigned int active_animation;
		unsigned int animation_num;

		//Animation
		Animation** animations;
		AnimationLst* ignored_animations;
};

typedef struct ObjectList{
	Object* obj;
	struct ObjectList* next;
} ObjectLst;

#endif