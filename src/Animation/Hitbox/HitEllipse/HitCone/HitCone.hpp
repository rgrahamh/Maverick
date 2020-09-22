#ifndef HITCONE_H
#define HITCONE_H

#include "../HitEllipse.hpp"

class HitCone : public HitEllipse{
	public:
		HitCone(float* x_base, float* y_base, float x_offset, float y_offset, float x_radius, float y_radius, float angle, float slice_prop, unsigned int type);
		~HitCone();

		void setScale(float x_scale, float y_scale);

		float getAngle();
		float getProp();

		bool checkCollision(Hitbox* other);
		void rotate(int direction, float rotate_dist);

	private:
		float angle;
		float slice_prop;
};

#include "../../HitboxCollision.hpp"

#endif