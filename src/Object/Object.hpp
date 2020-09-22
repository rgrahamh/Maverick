#ifndef OBJECT_H
#define OBJECT_H

#include "../Animation/Animation.hpp"
#include "../Animation/Hitbox/Hitbox.hpp"
#include "../Animation/Hitbox/HitRect/HitRect.hpp"
#include "../Animation/Hitbox/HitEllipse/HitEllipse.hpp"
#include "../Animation/Hitbox/HitEllipse/HitCone/HitCone.hpp"
#include "../Animation/Hitbox/HitboxCollision.hpp"

class Object{
	public:
		Object(float start_x, float start_y, float friction, unsigned int animation_num, bool animated = true);
		virtual ~Object();
		float getX();
		float getY();
		float getWidth();
		float getHeight();
		unsigned int getDrawLayer();
		HitboxLst* getHitboxes();

		void addSprite(unsigned int animation_num, const char* sprite_path, unsigned int keyframe, float x_offset, float y_offset);
		void addHitbox(unsigned int animation_num, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type, int sprite_num = -1);
		void addHitbox(unsigned int animation_num, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type, float angle, float slice_prop, int sprite_num = -1);

		void setAnimation(unsigned int animation_num);
		void setScale(unsigned int animation_num, float x_scale, float y_scale);
		void setScale(float x_scale, float y_scale);

		//Processing functions
		void _process();
		//Need this for custom processing (like input)
		virtual void process();

		//Useful for actions on other objects & input
		virtual void action(sf::Event event);

		virtual void draw(sf::RenderWindow* window);

		void _onCollide(Object* other, Hitbox* this_hitbox, Hitbox* other_hitbox);
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

		unsigned int draw_layer;
		unsigned int active_animation;
		unsigned int animation_num;

		//Animation
		Animation** animations;
};

typedef struct ObjectList{
	Object* obj;
	struct ObjectList* next;
} ObjectLst;

#endif