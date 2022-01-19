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

enum OBJECT_TYPE{
    GENERIC_OBJECT,
    CAMERA_REFERENCE,
	EXTENDED_OBJECT_TYPE_START = 4096
};

class Object : public Entity{
	public:
		Object(const char* name, float start_x, float start_y, float friction, float mass, int layers = 1);
		virtual ~Object();
		double getOldX();
		double getOldY();
		double getXVel();
		double getYVel();
		float getMass();
		bool getEnvBump();
		Sprite* getSprite();
		int getCollisionLayer();

		virtual int serializeExtendedAssets(FILE* file, std::unordered_set<std::string>& sprite_set, std::unordered_set<std::string>& audio_set, std::unordered_set<std::string>& music_set);
		virtual int serializeExtendedData(FILE* file, Zone* base_zone);

		void setXVel(double xV);
		void setYVel(double yV);
		void setEnvBump();
		void setFriction(float friction);
		void setCollisionLayer(int collision_layer);

		void applyForce(double xA, double yA);

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
		double old_x;
		double old_y;

		//Velocity
		double xV;
		double yV;

		//Acceleration
		double xA;
		double yA;

		//Coefficient of friction
		float friction;

		//How much the object weighs
		float mass;

		//Environmental bump
		bool env_bump;

		//The collision layer
		int collision_layer;
};

struct ObjectList{
	Object* obj;
	struct ObjectList* next;
};

#endif