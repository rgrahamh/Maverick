#ifndef HITRECT_H
#define HITRECT_H

#include "../Hitbox.hpp"

class HitRect : public Hitbox{
	public:
		HitRect(double* x_base, double* y_base, double x_offset, double y_offset, float width, float height, unsigned int type);
		~HitRect();

		float getWidth();
		float getHeight();

        float getRightBound() override;
        float getLeftBound() override;
        float getTopBound() override;
        float getBotBound() override;
		float getDrawAxis() override;

		void setScale(double x_scale, double y_scale) override;
		bool isPointInside(double x_coord, double y_coord) override;

		bool checkCollision(Hitbox* other) override;

	private:
		float curr_width;
		float curr_height;

		float base_width;
		float base_height;
};

#include "../HitboxCollision.hpp"

#endif