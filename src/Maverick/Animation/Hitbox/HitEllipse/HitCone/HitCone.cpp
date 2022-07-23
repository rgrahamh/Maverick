#include "./HitCone.hpp"

/** HitCone constructor
 * @param x_base A pointer to the base X component
 * @param y_base A pointer to the base Y component
 * @param z_base A pointer to the base Z component
 * @param x_offset The X offset of the hitbox
 * @param y_offset The Y offset of the hitbox
 * @param z_offset The Z offset of the hitbox
 * @param x_radius The X radius component of the cone
 * @param y_radius The Y radius component of the cone
 * @param angle The angle in the ellipse that the start of the cone is at
 * @param slice_prop The proportion of the circle
 * @param type Some HITBOX_TYPE vals or'd together
 * @param hitbox_group The hitbox group that this hitbox belongs to
 * @param immunity_timer The amount of time that something colliding with this hitbox should be immune to it
 */
HitCone::HitCone(double* x_base, double* y_base, double* z_base, double x_offset, double y_offset, double z_offset, double depth, double x_radius, double y_radius, float angle, float slice_prop, unsigned int type)
	:HitEllipse(x_base, y_base, z_base, x_offset, y_offset, z_offset, depth, x_radius, y_radius, type){
	this->angle = angle;
	this->slice_prop = slice_prop;
	this->shape = HIT_CONE;
}

/** HitCone destructor
 */
HitCone::~HitCone(){}

/** Gets the starting angle of the HitCone
 */
float HitCone::getAngle(){
	return this->angle;
}

/** Gets the slice proprotion
 */
float HitCone::getProp(){
	return this->slice_prop;
}

/** Checks collision between this and the other specified hitbox
 * @param other The other hibox
 * @return True if the hitbox is colliding with the other, false otherwise
 */
bool HitCone::checkCollision(Hitbox* other){
	switch (other->getShape()){
		case HIT_RECT:{
			return collisionRectCone((HitRect*)other, this);
		}
		break;

		case HIT_ELLIPSE:{
			return collisionEllipseCone((HitEllipse*)other, this);
		}
		break;

		case HIT_CONE:{
			return collisionConeCone((HitCone*)other, this);
		}
		break;
	}
	return false;
}

/** Rotates the hitbox slightly to the right (if direction is 0) or left (if direction is 1)
 * @param direction The direction that things are being rotated to
 * @param rotate_dist The distance you should rotate
 */
void HitCone::rotate(int direction, float rotate_dist){
	if(direction == 0){
		this->angle -= rotate_dist;
	}
	else{
		this->angle += rotate_dist;
	}
}