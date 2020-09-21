#ifndef HITCIRCLE_H
#define HITCIRCLE_H

#include "../Hitbox.hpp"

class HitEllipse : public Hitbox{
	public:
		HitEllipse(float* x_base, float* y_base, float x_offset, float y_offset, float x_radius, float y_radius);
		virtual ~HitEllipse();

		float getXRadius();
		float getYRadius();

		void setScale(float x_scale, float y_scale);
		virtual bool checkCollision(Hitbox* other);

	protected:
		float x_base_radius;
		float x_curr_radius;

		float y_base_radius;
		float y_curr_radius;
};

#include "../HitboxCollision.hpp"

#endif