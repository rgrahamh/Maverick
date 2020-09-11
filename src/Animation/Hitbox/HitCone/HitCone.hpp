#include "../Hitbox.hpp"

class HitCone : public Hitbox{
	public:
		HitCone(float x_offset, float y_offset, float radius, float angle, float slice_prop);
		~HitCone();

		void setScale(float x_scale, float y_scale);

		float getRadius();
		float getAngle();
		float getProp();

		void rotate(int direction);

	private:
		float base_radius;
		float curr_radius;
		float angle;
		float slice_prop;
};