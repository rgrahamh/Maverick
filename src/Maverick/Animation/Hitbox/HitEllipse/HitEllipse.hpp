#ifndef HITCIRCLE_H
#define HITCIRCLE_H

#include "../Hitbox.hpp"
#include <math.h>

class HitEllipse : public Hitbox{
	public:
		HitEllipse(double* x_base, double* y_base, double x_offset, double y_offset, double x_radius, double y_radius, unsigned int type, int32_t hitbox_group = -1, uint32_t immunity_timer = 0);
		virtual ~HitEllipse();

		float getXRadius();
		float getYRadius();

        float getRightBound() override;
        float getLeftBound() override;
        float getTopBound() override;
        float getBotBound() override;
		float getDrawAxis() override;

		void setScale(double x_scale, double y_scale) override;
		virtual bool checkCollision(Hitbox* other) override;

		virtual bool isPointInside(double x_coord, double y_coord) override;

		void serializeData(FILE* file);

	protected:
		double x_base_radius;
		double x_curr_radius;

		double y_base_radius;
		double y_curr_radius;
};

#include "../HitboxCollision.hpp"

#endif