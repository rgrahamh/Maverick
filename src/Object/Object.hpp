#ifndef OBJECT_H
#define OBJECT_H

#include "../Animation/Animation.hpp"
#include "./Hitbox/Hitbox.hpp"

class Object{
	public:
		Object(float start_x, float start_y, float friction, unsigned int draw_layer, unsigned int animation_num);
		virtual ~Object();
		float getX();
		float getY();
		float getWidth();
		float getHeight();
		unsigned int getDrawLayer();

		void addSprite(unsigned int animation_num, const char* sprite_path, unsigned int keyframe, float x_offset, float y_offset);

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

		
	protected:
		//Position
		float x;
		float y;

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

		//Hitboxes
		Hitbox** hitboxes;

		//Animation
		Animation** animations;
};

typedef struct ObjectList{
	Object* obj;
	struct ObjectList* next;
} ObjectLst;

#endif