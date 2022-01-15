#include "./Hitbox.hpp"

/** The hitbox constructor
 * @param x_base A pointer to the base object's X value
 * @param y_base A pointer to the base object's Y value
 * @param x_offset The hitbox offset from the base X position
 * @param y_offset The hitbox offset from the base Y position
 */
Hitbox::Hitbox(double* x_base, double* y_base, double x_offset, double y_offset, uint64_t type, int32_t hitbox_group, int32_t hitbox_disable_timer){
	this->x_base = x_base;
	this->y_base = y_base;
	this->x_base_offset = x_offset;
	this->y_base_offset = y_offset;
	this->x_curr_offset = x_offset;
	this->y_curr_offset = y_offset;
	this->type = type;
	this->hitbox_group = hitbox_group;
	this->hitbox_disable_timer = hitbox_disable_timer;
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
void Hitbox::setScale(double x_scale, double y_scale){
	this->x_curr_offset = x_scale * x_base_offset;
	this->y_curr_offset = y_scale * y_base_offset;
}

/** Returns true if the point is inside the hitbox, false otherwise. In the base class, it always returns false (it should always be overridden)
 * @return If the point is inside the hitbox
 */
bool Hitbox::isPointInside(double x_coord, double y_coord){
	return false;
}

/** Returns true if this collisdes with the other hitbox, false otherwise. In the base class, it always returns false (it should always be overridden)
 * @param other The other hitbox collision is being checked against
 * @return If the point is inside the hitbox
 */
bool Hitbox::checkCollision(Hitbox* other){
	return false;
}

/** Gets the top hitbox bound
 * @return The top hitbox bound
 */
float Hitbox::getTopBound(){
	return *this->y_base + this->y_curr_offset;
}

/** Gets the bottom hitbox bound
 * @return The bottom hitbox bound
 */
float Hitbox::getBotBound(){
	return *this->y_base + this->y_curr_offset;
}

/** Gets the left hitbox bound
 * @return The left hitbox bound
 */
float Hitbox::getLeftBound(){
	return *this->x_base + this->x_curr_offset;
}

/** Gets the right hitbox bound
 * @return The right hitbox bound
 */
float Hitbox::getRightBound(){
	return *this->x_base + this->x_curr_offset;
}

/** Gets the draw axis of the hitbox
 * @return The draw axis of the hitbox
 */
float Hitbox::getDrawAxis(){
	return *this->y_base + this->y_curr_offset;
}

/** Gets the hitbox group that the hitbox belongs to
 * @return The hitbox group that the hitbox belongs to
 */
int32_t Hitbox::getHitboxGroup(){
	return this->hitbox_group;
}

/** Gets the time (in ms) that the other object should not
 *  check for collision with the hitbox group that
 *  this hitbox belongs to (in the parent object)
 * @return The hitbox group that the hitbox belongs to
 */
int32_t Hitbox::getHitboxDisableTimer(){
	return this->hitbox_disable_timer;
}