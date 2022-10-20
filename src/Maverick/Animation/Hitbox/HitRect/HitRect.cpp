#include "./HitRect.hpp"
#include "Maverick/FileHandler/FileHandler.hpp"

/** HitRect constructor
 * @param x_base A pointer to the base X component
 * @param y_base A pointer to the base Y component
 * @param z_base A pointer to the base Z component
 * @param x_offset The X offset of the hitbox
 * @param y_offset The Y offset of the hitbox
 * @param z_offset The Z offset of the hitbox
 * @param width The width of the hitbox
 * @param height The height of the hitbox
 * @param depth The depth of the hitbox
 * @param type The HITBOX_TYPE attributes
 * @param hitbox_group The hitbox group that this hitbox belongs to
 * @param immunity_timer The amount of time that something colliding with this hitbox should be immune to it
 */
HitRect::HitRect(double* x_base, double* y_base, double* z_base, double x_offset, double y_offset, double z_offset, float width, float height, double depth, unsigned int type, int32_t hitbox_group, uint32_t immunity_timer)
	:Hitbox(x_base, y_base, z_base, x_offset, y_offset, z_offset, depth, type, hitbox_group, immunity_timer){
	this->base_height = height;
	this->curr_height = height;
	this->base_width = width;
	this->curr_width = width;
	this->shape = HIT_RECT;
}

/** The destructor for HitRect
 */
HitRect::~HitRect(){}

/** Gets the height of the hitbox
 * @return The height of the hitbox
 */
double HitRect::getHeight(){
	return this->curr_height;
}

/** Gets the width of the hitbox
 * @return The width of the hitbox
 */
double HitRect::getWidth(){
	return this->curr_width;
}

/** Checks if the point is inside the hitbox
 * @param x_coord The X coordinate to check against
 * @param y_coord The Y coordinate to check against
 * @return true if the point is inside the hitbox, false otherwise
 */
bool HitRect::isPointInside(double x_coord, double y_coord){
	if(x_coord > *this->x_base + this->x_offset
	&& x_coord < *this->x_base + this->x_offset + this->curr_width
	&& y_coord > *this->y_base + this->y_offset
	&& y_coord < *this->y_base + this->y_offset + this->curr_height){
		return true;
	}
	return false;
}

/** Checks collision with another hitbox
 * @param other The other hitbox
 * @return True if the hitbox is colliding with the other, false otherwise
 */
bool HitRect::checkCollision(Hitbox* other){
	switch (other->getShape()){
		case HIT_RECT:
			return collisionRectRect(this, (HitRect*)other);
			break;

		case HIT_ELLIPSE:
			return collisionRectEllipse(this, (HitEllipse*)other);
			break;

		case HIT_CONE:
			return collisionRectCone(this, (HitCone*)other);
			break;
	}
	return false;
}

/** Gets the top hitbox bound
 * @return The top hitbox bound
 */
float HitRect::getTopBound(){
	return *this->y_base + this->y_offset;
}

/** Gets the bottom hitbox bound
 * @return The bottom hitbox bound
 */
float HitRect::getBotBound(){
	return *this->y_base + this->y_offset + this->curr_height;
}

/** Gets the left hitbox bound
 * @return The left hitbox bound
 */
float HitRect::getLeftBound(){
	return *this->x_base + this->x_offset;
}

/** Gets the right hitbox bound
 * @return The right hitbox bound
 */
float HitRect::getRightBound(){
	return *this->x_base + this->x_offset + this->curr_width;
}

/** Serialize the HitRect
 * @param file The file that serialized data is outputted to
 */
void HitRect::serializeData(FILE* file){
	//Write hitbox shape
	uint8_t shape = this->shape;
	fwrite(&shape, 1, 1, file);

	//Write X & Y offsets
	WriteVar((uint64_t)this->x_offset, file);
	WriteVar((uint64_t)this->y_offset, file);

	//Write X & Y components (widths)
	WriteVar((uint64_t)this->base_width, file);
	WriteVar((uint64_t)this->base_height, file);

	//Hitbox flags
	WriteVar(this->type, file);

	//Hitbox group
	WriteVar(this->hitbox_group, file);

	//Immunity timer
	WriteVar(this->immunity_timer, file);
}