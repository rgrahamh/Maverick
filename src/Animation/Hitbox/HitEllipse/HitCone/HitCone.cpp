#include "./HitCone.hpp"

/** HitCone constructor
 * @param x_base A pointer to the base object's X location
 * @param y_base A pointer to the base object's Y location
 * @param x_offset The X offset of the HitCone
 * @param y_offset The Y offset of the HitCone
 * @param x_radius The X radius component of the cone
 * @param y_radius The Y radius component of the cone
 * @param angle The angle in the ellipse that the start of the cone is at
 * @param slice_prop The proportion of the circle
 * @param type Some HITBOX_TYPE vals or'd together
 */
HitCone::HitCone(float* x_base, float* y_base, float x_offset, float y_offset, float x_radius, float y_radius, float angle, float slice_prop, unsigned int type)
	:HitEllipse(x_base, y_base, x_offset, y_offset, x_radius, y_radius, type){
	this->angle = angle;
	this->slice_prop = slice_prop;
	this->shape = CONE;
}

/** HitCone destructor
 */
HitCone::~HitCone(){}

/** Sets the scale of the object
 * @param x_scale The new X scale component
 * @param y_scale The new Y scale component
 */
void HitCone::setScale(float x_scale, float y_scale){
	this->x_curr_offset = x_scale * this->x_base_offset;
	this->y_curr_offset = y_scale * this->y_base_offset;
}

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
		case RECT:{
			return collisionRectCone((HitRect*)other, this);
		}
		break;

		case ELLIPSE:{
			return collisionEllipseCone((HitEllipse*)other, this);
		}
		break;

		case CONE:{
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