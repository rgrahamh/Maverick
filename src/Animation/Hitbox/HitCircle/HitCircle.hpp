#include "../Hitbox.hpp"

class HitCircle : public Hitbox{
	public:
		HitCircle(float x_offset, float y_offset, float radius);
		~HitCircle();

		float getRadius();

		void setScale(float x_scale, float y_scale);

	private:
		float base_radius;
		float curr_radius;
};