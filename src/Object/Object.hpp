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
		Object(float start_x, float start_y, unsigned int draw_layer);
		~Object();
		float getX();
		float getY();

		float getWidth();
		float getHeight();

		//Processing functions
		void _process();
		//Need this for custom processing (like input)
		void process();

		void _draw(sf::RenderWindow* window, unsigned int draw_layer);

		//Need this later for drawing weapons and armor (calling their draw functions)
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

		//Height and width values
		float height;
		float width;

		unsigned int draw_layer;

		//Coefficient of friction
		float friction;

		//Hitboxes
		Hitbox** hitboxes;

		//Animation
		Animation* animation;
};

#endif