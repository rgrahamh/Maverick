#ifndef HITRECT_H
#define HITRECT_H

#include "../Hitbox.hpp"

class HitRect : public Hitbox{
	public:
		HitRect(float* x_base, float* y_base, float x_offset, float y_offset, float width, float height, unsigned int type);
		~HitRect();

		float getWidth();
		float getHeight();

		void setScale(float x_scale, float y_scale);
		bool isPointInside(float x_coord, float y_coord);

		bool checkCollision(Hitbox* other);

	private:
		float curr_width;
		float curr_height;

		float base_width;
		float base_height;
};

#include "../HitboxCollision.hpp"

#endif