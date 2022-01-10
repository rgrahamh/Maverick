#ifndef HITCONE_H
#define HITCONE_H

#include "../HitEllipse.hpp"

class HitCone : public HitEllipse{
	public:
		HitCone(double* x_base, double* y_base, double x_offset, double y_offset, double x_radius, double y_radius, float angle, float slice_prop, unsigned int type);
		~HitCone();

		void setScale(double x_scale, double y_scale);

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