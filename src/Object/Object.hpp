#ifndef OBJECT_H
#define OBJECT_H

#include "../Animation/Animation.hpp"
#include "../Animation/Hitbox/Hitbox.hpp"
#include "../Animation/Hitbox/HitRect/HitRect.hpp"
#include "../Animation/Hitbox/HitEllipse/HitEllipse.hpp"
#include "../Animation/Hitbox/HitEllipse/HitCone/HitCone.hpp"
#include "../Animation/Hitbox/HitboxCollision.hpp"
#include "../Control/Control.hpp"

#include <SDL2/SDL.h>
#include <unordered_map>

class Object{
	public:
		Object(const char* name, float start_x, float start_y, float friction, float mass, unsigned int animation_num, int draw_layer = 1);
		virtual ~Object();
		char* getName();
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
		void* getAttr(const char* key);

		void addSprite(unsigned int animation_num, const char* sprite_path, unsigned int keytime, float x_offset, float y_offset);

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
		void setVisible(bool visible);
		void setEnvBump();
		void setAttr(const char* key, void* val);

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
		//Name
		char* name;

		//Attributes
		HashTable* attr;

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