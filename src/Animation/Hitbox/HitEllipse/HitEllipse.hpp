#ifndef HITCIRCLE_H
#define HITCIRCLE_H

#include "../Hitbox.hpp"
#include <math.h>

class HitEllipse : public Hitbox{
	public:
		HitEllipse(float* x_base, float* y_base, float x_offset, float y_offset, float x_radius, float y_radius, unsigned int type);
		virtual ~HitEllipse();

		float getXRadius();
		float getYRadius();

        float getRightBound() override;
        float getLeftBound() override;
        float getTopBound() override;
        float getBotBound() override;
		float getDrawAxis() override;

		void setScale(float x_scale, float y_scale) override;
		virtual bool checkCollision(Hitbox* other) override;

		virtual bool isPointInside(float x_coord, float y_coord) override;

	protected:
		float x_base_radius;
		float x_curr_radius;

		float y_base_radius;
		float y_curr_radius;
};

#include "../HitboxCollision.hpp"

#endif