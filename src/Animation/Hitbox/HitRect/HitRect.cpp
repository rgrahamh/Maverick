#include "./HitRect.hpp"

/** HitRect constructor
 * @param x_base A pointer to the base X component
 * @param y_base A pointer to the base Y component
 * @param x_offset The X offset of hitbox
 * @param y_offset The Y offset of hitbox
 * @param width The width of the hitbox
 * @param height The height of the hitbox
 * @param type The HITBOX_TYPE attributes
 */
HitRect::HitRect(float* x_base, float* y_base, float x_offset, float y_offset, float width, float height, unsigned int type)
	:Hitbox(x_base, y_base, x_offset, y_offset, type){
		this->base_height = height;
		this->curr_height = height;
		this->base_width = width;
		this->curr_width = width;
		this->shape = RECT;
}

/** The destructor for HitRect
 */
HitRect::~HitRect(){}

/** Gets the height of the hitbox
 * @return The height of the hitbox
 */
float HitRect::getHeight(){
	return this->curr_height;
}

/** Gets the width of the hitbox
 * @return The width of the hitbox
 */
float HitRect::getWidth(){
	return this->curr_width;
}

/** Checks if the point is inside the hitbox
 * @param x_coord The X coordinate to check against
 * @param y_coord The Y coordinate to check against
 * @return true if the point is inside the hitbox, false otherwise
 */
bool HitRect::isPointInside(float x_coord, float y_coord){
	if(x_coord > *this->x_base + this->x_curr_offset
	&& x_coord < *this->x_base + this->x_curr_offset + this->curr_width
	&& y_coord > *this->y_base + this->y_curr_offset
	&& y_coord < *this->y_base + this->y_curr_offset + this->curr_height){
		return true;
	}
	return false;
}

/** Sets the scale of the HitRect
 * @param x_scale The X scale of the hitbox
 * @param y_scale The Y scale of the hitbox
 */
void HitRect::setScale(float x_scale, float y_scale){
	Hitbox::setScale(x_scale, y_scale);
	this->curr_height = y_scale * this->base_height;
	this->curr_width = x_scale * this->base_width;
}

/** Checks collision with another hitbox
 * @param other The other hitbox
 * @return True if the hitbox is colliding with the other, false otherwise
 */
bool HitRect::checkCollision(Hitbox* other){
	switch (other->getShape()){
		case RECT:
			return collisionRectRect(this, (HitRect*)other);
			break;

		case ELLIPSE:
			return collisionRectEllipse(this, (HitEllipse*)other);
			break;

		case CONE:
			return collisionRectCone(this, (HitCone*)other);
			break;
	}
	return false;
}

/** Gets the top bound of the rectangle
 */
float HitRect::getTopBound(){
	return *this->y_base + this->y_curr_offset;
}

/** Gets the bottom bound of the rectangle
 */
float HitRect::getBotBound(){
	return *this->y_base + this->y_curr_offset + this->curr_height;
}

/** Gets the left bound of the rectangle
 */
float HitRect::getLeftBound(){
	return *this->x_base + this->x_curr_offset;
}

/** Gets the right bound of the rectangle
 */
float HitRect::getRightBound(){
	return *this->x_base + this->x_curr_offset + this->curr_width;
}