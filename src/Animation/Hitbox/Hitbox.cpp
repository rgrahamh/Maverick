#include "./Hitbox.hpp"

/** The hitbox constructor
 * @param x_base A pointer to the base object's X value
 * @param y_base A pointer to the base object's Y value
 * @param x_offset The hitbox offset from the base X position
 * @param y_offset The hitbox offset from the base Y position
 */
Hitbox::Hitbox(float* x_base, float* y_base, float x_offset, float y_offset, unsigned int type){
	this->x_base = x_base;
	this->y_base = y_base;
	this->x_base_offset = x_offset;
	this->y_base_offset = y_offset;
	this->x_curr_offset = x_offset;
	this->y_curr_offset = y_offset;
	this->type = type;
}

/** The hitbox deconstructor
 */
Hitbox::~Hitbox(){}

/** Returns the X value of the hitbox
 * @return The X value of the hitbox
 */
float Hitbox::getX(){
	return *this->x_base + this->x_curr_offset;
}

/** Returns the Y value of the hitbox
 * @return The Y value of the hitbox
 */
float Hitbox::getY(){
	return *this->y_base + this->y_curr_offset;
}

/** Returns the shape of hitbox
 * @return The HITBOX_SHAPE that represents the hitbox shape
 */
HITBOX_SHAPE Hitbox::getShape(){
	return this->shape;
}

/** Returns the type of the hitbox
 * @return The HITBOX_TYPE that represents the hitbox type
 */
unsigned int Hitbox::getType(){
	return this->type;
}

/** Sets the scale of the hitbox
 * @param x_scale The new X scale of the hitbox
 * @param y_scale The new Y scale of the hitbox
 */
void Hitbox::setScale(float x_scale, float y_scale){
	this->x_curr_offset = x_scale * x_base_offset;
	this->y_curr_offset = y_scale * y_base_offset;
}

/** Returns true if the point is inside the hitbox, false otherwise. In the base class, it always returns false (it should always be overridden)
 * @return If the point is inside the hitbox
 */
bool Hitbox::isPointInside(float x_coord, float y_coord){
	return false;
}

/** Returns true if this collisdes with the other hitbox, false otherwise. In the base class, it always returns false (it should always be overridden)
 * @param other The other hitbox collision is being checked against
 * @return If the point is inside the hitbox
 */
bool Hitbox::checkCollision(Hitbox* other){
	return false;
}

float Hitbox::getTopBound(){
	return *this->y_base + this->y_curr_offset;
}

float Hitbox::getBotBound(){
	return *this->y_base + this->y_curr_offset;
}

float Hitbox::getRightBound(){
	return *this->x_base + this->x_curr_offset;
}

float Hitbox::getLeftBound(){
	return *this->x_base + this->x_curr_offset;
}