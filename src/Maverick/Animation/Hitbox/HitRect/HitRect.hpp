#ifndef HITRECT_H
#define HITRECT_H

#include "../Hitbox.hpp"

class HitRect : public Hitbox{
	public:
		HitRect(double* x_base, double* y_base, double x_offset, double y_offset, float width, float height, unsigned int type, int32_t hitbox_group = -1, uint32_t immunity_timer = 0);
		~HitRect();

		double getWidth();
		double getHeight();

        float getRightBound() override;
        float getLeftBound() override;
        float getTopBound() override;
        float getBotBound() override;
		float getDrawAxis() override;

		void setScale(double x_scale, double y_scale) override;
		bool isPointInside(double x_coord, double y_coord) override;

		bool checkCollision(Hitbox* other) override;

		void serializeData(FILE* file);

	private:
		double curr_width;
		double curr_height;

		double base_width;
		double base_height;
};

#include "../HitboxCollision.hpp"

#endif