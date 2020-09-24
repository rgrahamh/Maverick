#include "./HitEllipse.hpp"

/** HitEllipse constructor
 * @param x_base A pointer to the base X component
 * @param y_base A pointer to the base Y component
 * @param x_offset The X offset of hitbox
 * @param y_offset The Y offset of hitbox
 * @param width The width of the hitbox
 * @param height The height of the hitbox
 * @param type The HITBOX_TYPE attributes
 */
HitEllipse::HitEllipse(float* x_base, float* y_base, float x_offset, float y_offset, float x_radius, float y_radius, unsigned int type)
	:Hitbox(x_base, y_base, x_offset, y_offset, type){
	this->x_base_radius = x_radius;
	this->y_base_radius = y_radius;
	this->x_curr_radius = x_radius;
	this->y_curr_radius = y_radius;
	this->shape = ELLIPSE;
}

/** HitEllipse destructor
 */
HitEllipse::~HitEllipse(){}

/** Gets the X component of the radius
 * @return The X component of the radius
 */
float HitEllipse::getXRadius(){
	return this->x_curr_radius;
}

/** Gets the Y component of the radius
 * @return The Y component of the radius
 */
float HitEllipse::getYRadius(){
	return this->y_curr_radius;
}

/** Sets the scale of the HitRect
 * @param x_scale The X scale of the hitbox
 * @param y_scale The Y scale of the hitbox
 */
void HitEllipse::setScale(float x_scale, float y_scale){
	this->x_curr_offset = x_scale * this->x_base_offset;
	this->y_curr_offset = y_scale * this->y_base_offset;
	this->x_curr_radius = (x_scale / 2.0f) * this->x_base_radius;
	this->y_curr_radius = (x_scale / 2.0f) * this->y_base_radius;
}

/** Checks collision against the other hitbox
 * @param other The other hitbox
 * @return True if the hitbox is colliding with the other, false otherwise
 */
bool HitEllipse::checkCollision(Hitbox* other){
	switch (other->getShape()){
		case RECT:
			return collisionRectEllipse((HitRect*)other, this);
			break;

		case ELLIPSE:
			return collisionEllipseEllipse(this, (HitEllipse*)other);
			break;

		case CONE:
			return collisionEllipseCone(this, (HitCone*)other);
			break;
	}
	return false;
}

/** Gets the top bound of the rectangle
 */
float HitEllipse::getTopBound(){
	return *this->y_base + this->y_curr_offset - this->y_curr_radius;
}

/** Gets the bottom bound of the rectangle
 */
float HitEllipse::getBotBound(){
	return *this->y_base + this->y_curr_offset + this->y_curr_radius;
}

/** Gets the left bound of the rectangle
 */
float HitEllipse::getLeftBound(){
	return *this->x_base + this->x_curr_offset - this->x_curr_radius;
}

/** Gets the right bound of the rectangle
 */
float HitEllipse::getRightBound(){
	return *this->x_base + this->x_curr_offset + this->x_curr_radius;
}