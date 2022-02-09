#include "./Hitbox.hpp"

/** The hitbox constructor
 * @param x_base A pointer to the base object's X value
 * @param y_base A pointer to the base object's Y value
 * @param z_base A pointer to the base object's Z value
 * @param x_offset The hitbox offset from the base X position
 * @param y_offset The hitbox offset from the base Y position
 * @param z_offset The hitbox offset from the base Z position
 * @param type The hitbox type (flags to specify what kind of hitbox it is)
 * @param group The hitbox group (other hitboxes that should be tied in with immunity will have the same value)
 * @param immunity_timer The amount of time the hitbox won't interact with a given "other" object if hit
 */
Hitbox::Hitbox(double* x_base, double* y_base, double* z_base, double x_offset, double y_offset, double z_offset, double depth, uint64_t type, int32_t hitbox_group, int32_t immunity_timer){
	this->x_base = x_base;
	this->y_base = y_base;
	this->z_base = z_base;
	this->x_offset = x_offset;
	this->y_offset = y_offset;
	this->z_offset = z_offset;
	this->depth = depth;
	this->type = type;
	this->hitbox_group = hitbox_group;
	this->immunity_timer = immunity_timer;
}

/** The hitbox deconstructor
 */
Hitbox::~Hitbox(){}

/** Returns the X value of the hitbox
 * @return The X value of the hitbox
 */
double Hitbox::getX(){
	return *this->x_base + this->x_offset;
}

/** Returns the Y value of the hitbox
 * @return The Y value of the hitbox
 */
double Hitbox::getY(){
	return *this->y_base + this->y_offset;
}

/** Returns the Z value of the hitbox
 * @return The Z value of the hitbox
 */
double Hitbox::getZ(){
	return *this->z_base + this->z_offset;
}

/** Returns the max Z value of the hitbox
 * @return The max Z value of the hitbox
 */
double Hitbox::getZMax(){
	return this->getZ() + this->depth;
}

/** Returns the min Z value of the hitbox
 * @return The min Z value of the hitbox
 */
double Hitbox::getZMin(){
	return this->getZ();
}

/** Returns the depth of the hitbox
 * @return The depth of the hitbox
 */
double Hitbox::getDepth(){
	return this->depth;
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
uint64_t Hitbox::getType(){
	return this->type;
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
	return *this->y_base + this->y_offset;
}

/** Gets the bottom hitbox bound
 * @return The bottom hitbox bound
 */
float Hitbox::getBotBound(){
	return *this->y_base + this->y_offset;
}

/** Gets the left hitbox bound
 * @return The left hitbox bound
 */
float Hitbox::getLeftBound(){
	return *this->x_base + this->x_offset;
}

/** Gets the right hitbox bound
 * @return The right hitbox bound
 */
float Hitbox::getRightBound(){
	return *this->x_base + this->x_offset;
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
int32_t Hitbox::getImmunityTimer(){
	return this->immunity_timer;
}