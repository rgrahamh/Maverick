#include "../Hitbox.hpp"

class HitRect : public Hitbox{
	public:
		HitRect(float x_offset, float y_offset, float width, float height);
		~HitRect();

		float getWidth();
		float getHeight();

		void setScale(float x_scale, float y_scale);

	private:
		float width;
		float height;
};