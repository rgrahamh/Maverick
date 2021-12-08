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
	Hitbox::setScale(x_scale, y_scale);
	this->x_curr_radius = x_scale * this->x_base_radius;
	this->y_curr_radius = y_scale * this->y_base_radius;
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
 * @return The top bound of the rectangle
 */
float HitEllipse::getTopBound(){
	return *this->y_base + this->y_curr_offset - this->y_curr_radius;
}

/** Gets the bottom bound of the rectangle
 * @return The bottom bound of the rectangle
 */
float HitEllipse::getBotBound(){
	return *this->y_base + this->y_curr_offset + this->y_curr_radius;
}

/** Gets the left bound of the rectangle
 * @return The left bound of the rectangle
 */
float HitEllipse::getLeftBound(){
	return *this->x_base + this->x_curr_offset - this->x_curr_radius;
}

/** Gets the right bound of the rectangle
 * @return The right bound of the rectangle
 */
float HitEllipse::getRightBound(){
	return *this->x_base + this->x_curr_offset + this->x_curr_radius;
}

/** Gets the draw axis of the hitbox
 * @return The draw axis of the hitbox
 */
float HitEllipse::getDrawAxis(){
	return *this->y_base + this->y_curr_offset;
}

/** Get if a point is inside of the hitbox
 * @param x_coord The X coordinate being tested against
 * @param y_coord The Y coordinate being tested against
 * @return If the point is inside of the hitbox
 */
bool HitEllipse::isPointInside(float x_coord, float y_coord){
	float x_center = this->getX();
	float y_center = this->getY();

	float x_diff = abs(x_center - x_coord);
	float y_diff = abs(y_center - y_coord);

	//Avoiding divide by zero
	if(x_diff == 0){
		if(y_diff < y_curr_radius){
			return true;
		}
	}
	else{
		float angle = atan(y_diff / x_diff);

		if(x_curr_radius * cos(angle) > x_diff && y_curr_radius * sin(angle) > y_diff){
			return true;
		}
	}
	return false;
}