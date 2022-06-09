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

//This leaves room for plenty of objects to grow
enum OBJECT_TYPE{
    GENERIC_OBJECT = 0x10000000,
    CAMERA_REFERENCE,
	EXTENDED_OBJECT_TYPE_START = 0x10000800,
	OBJECT_END   = 0x2FFFFFFF
};

class Object : public Entity{
	public:
		Object(const char* name, float start_x, float start_y, float start_z, float friction, float mass, float terminal_velocity, bool gravity = false, int layer = 1);
		virtual ~Object();
		double getOldX();
		double getOldY();
		double getOldZ();
		double getXVel();
		double getYVel();
		double getZVel();
		double getZ();
		float getMass();
		bool getEnvBump();
		Sprite* getSprite();
		int getCollisionLayer();
		float getTerminalVelocity();
		double getGround();
		double getNextGround();

		virtual int serializeExtendedAssets(FILE* file, SerializeSet& serialize_set) override;
		virtual int serializeExtendedData(FILE* file, Zone* base_zone) override;

		virtual int deserializeExtendedData(FILE* file);

		void setXVel(double xV);
		void setYVel(double yV);
		void setZVel(double zV);
		void setZ(double z);
		void setGravity(bool gravity);
		void setEnvBump();
		void setFriction(float friction);
		void setCollisionLayer(int collision_layer);
		void setGround(double next_ground);
		void setNextGround(double next_ground);

		void applyForce(double xA, double yA, double zA = 0);

		//Processing functions
		virtual void _process(uint64_t delta, unsigned int steps) override;
		//Need this for custom processing
		virtual void process(uint64_t delta, unsigned int steps) override;

		virtual void _action(Control* control) override;
		virtual void action(Control* control) override;

		virtual void _draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y) override;
		virtual void draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y) override;

		virtual void onCollide(Object* other, Hitbox* this_hitbox, Hitbox* other_hitbox);

	protected:
		//Previous position (used for rollback)
		double old_x;
		double old_y;
		double old_z;

		//The Z position of the ground
		double ground;
		double next_ground;

		//Velocity
		double xV;
		double yV;
		double zV;

		//Acceleration
		double xA;
		double yA;
		double zA;

		//Coefficient of friction
		float friction;

		//How much the object weighs
		float mass;

		//Environmental bump
		bool env_bump;

		//If gravity should be applied to this object
		bool gravity;

		//Velocity cap
		float terminal_velocity;

		//The collision layer
		int collision_layer;
};

struct ObjectList{
	Object* obj;
	struct ObjectList* next;
};

#endif