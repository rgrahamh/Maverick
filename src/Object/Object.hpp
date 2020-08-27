#ifndef OBJECT_H
#define OBJECT_H

#include "../Animation/Animation.hpp"
#include "./Hitbox/Hitbox.hpp"

typedef struct ObjectList{
	Object* obj;
	ObjLst* next;
} ObjLst;

class Object{
	public:
		Object(float start_x, float start_y);
		~Object();
		float getX();
		float getY();

		//Processing functions
		void _process();
		//Need this for custom processing (like input)
		void process();

		//Drawing functions
		void _draw();
		//Need this later for drawing weapons and armor (calling their draw functions)
		void draw();

		
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

		unsigned int draw_layer;

		//Coefficient of friction
		float friction;

		//Hitboxes
		Hitbox** hitboxes;

		//Animation
		Animation* animation;
};

#endif