#include "./HitEllipse.hpp"
#include "Maverick/FileHandler/FileHandler.hpp"

/** HitEllipse constructor
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
HitEllipse::HitEllipse(double* x_base, double* y_base, double* z_base, double x_offset, double y_offset, double z_offset, double x_radius, double y_radius, double depth, unsigned int type, int32_t hitbox_group, uint32_t immunity_timer)
	:Hitbox(x_base, y_base, z_base, x_offset, y_offset, z_offset, depth, type, hitbox_group, immunity_timer){
	this->x_base_radius = x_radius;
	this->y_base_radius = y_radius;
	this->x_curr_radius = x_radius;
	this->y_curr_radius = y_radius;
	this->shape = HIT_ELLIPSE;
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

/** Checks collision against the other hitbox
 * @param other The other hitbox
 * @return True if the hitbox is colliding with the other, false otherwise
 */
bool HitEllipse::checkCollision(Hitbox* other){
	switch (other->getShape()){
		case HIT_RECT:
			return collisionRectEllipse((HitRect*)other, this);
			break;

		case HIT_ELLIPSE:
			return collisionEllipseEllipse(this, (HitEllipse*)other);
			break;

		case HIT_CONE:
			return collisionEllipseCone(this, (HitCone*)other);
			break;
	}
	return false;
}

/** Gets the top bound of the rectangle
 * @return The top bound of the rectangle
 */
float HitEllipse::getTopBound(){
	return *this->y_base + this->y_offset - this->y_curr_radius;
}

/** Gets the bottom bound of the rectangle
 * @return The bottom bound of the rectangle
 */
float HitEllipse::getBotBound(){
	return *this->y_base + this->y_offset + this->y_curr_radius;
}

/** Gets the left bound of the rectangle
 * @return The left bound of the rectangle
 */
float HitEllipse::getLeftBound(){
	return *this->x_base + this->x_offset - this->x_curr_radius;
}

/** Gets the right bound of the rectangle
 * @return The right bound of the rectangle
 */
float HitEllipse::getRightBound(){
	return *this->x_base + this->x_offset + this->x_curr_radius;
}

/** Get if a point is inside of the hitbox
 * @param x_coord The X coordinate being tested against
 * @param y_coord The Y coordinate being tested against
 * @return If the point is inside of the hitbox
 */
bool HitEllipse::isPointInside(double x_coord, double y_coord){
	double x_center = this->getX();
	double y_center = this->getY();

	double x_diff = abs(x_center - x_coord);
	double y_diff = abs(y_center - y_coord);

	//Avoiding divide by zero
	if((x_diff == 0 && y_diff < y_curr_radius) ||
	   (x_curr_radius > x_diff && y_curr_radius > y_diff)){
		return true;
	}
	return false;
}

/** Serialize the HitRect
 * @param file The file that serialized data is outputted to
 */
void HitEllipse::serializeData(FILE* file){
	//Write hitbox shape
	uint8_t shape = this->shape;
	fwrite(&shape, 1, 1, file);

	//Write X & Y offsets
	WriteVar((uint64_t)this->x_offset, file);
	WriteVar((uint64_t)this->y_offset, file);

	//Write X & Y components (radii)
	WriteVar((uint64_t)this->x_base_radius, file);
	WriteVar((uint64_t)this->y_base_radius, file);

	//Hitbox flags
	WriteVar(this->type, file);

	//Hitbox group
	WriteVar(this->hitbox_group, file);

	//Immunity timer
	WriteVar(this->immunity_timer, file);
}