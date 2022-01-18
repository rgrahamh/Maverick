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
HitEllipse::HitEllipse(double* x_base, double* y_base, double x_offset, double y_offset, double x_radius, double y_radius, unsigned int type, int32_t hitbox_group, uint32_t immunity_timer)
	:Hitbox(x_base, y_base, x_offset, y_offset, type, hitbox_group, immunity_timer){
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
void HitEllipse::setScale(double x_scale, double y_scale){
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
bool HitEllipse::isPointInside(double x_coord, double y_coord){
	double x_center = this->getX();
	double y_center = this->getY();

	double x_diff = abs(x_center - x_coord);
	double y_diff = abs(y_center - y_coord);

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

/** Serialize the HitRect
 * @param file The file that serialized data is outputted to
 */
void HitEllipse::serializeData(FILE* file){
	//Write hitbox shape
	uint8_t shape = this->shape;
	fwrite(&shape, 1, 1, file);

	//Write X offset
	uint64_t x_offset_swap = EndianSwap((uint64_t*)&this->x_base_offset);
	fwrite(&x_offset_swap, sizeof(x_offset_swap), 1, file);

	//Write Y offset
	uint64_t y_offset_swap = EndianSwap((uint64_t*)&this->y_base_offset);
	fwrite(&y_offset_swap, sizeof(y_offset_swap), 1, file);

	//Write X component (X radius/width)
	uint64_t width_swap = EndianSwap((uint64_t*)&this->x_base_radius);
	fwrite(&width_swap, sizeof(width_swap), 1, file);

	//Write Y component (Y radius/height)
	uint64_t height_swap = EndianSwap((uint64_t*)&this->y_base_radius);
	fwrite(&height_swap, sizeof(height_swap), 1, file);

	//Hitbox flags
	uint64_t type_swap = EndianSwap(&this->type);
	fwrite(&type_swap, sizeof(type_swap), 1, file);

	//Hitbox group
	uint32_t hitbox_group_swap = EndianSwap(&this->hitbox_group);
	fwrite(&hitbox_group_swap, sizeof(hitbox_group_swap), 1, file);

	//Immunity timer
	uint32_t immunity_timer_swap = EndianSwap(&this->immunity_timer);
	fwrite(&immunity_timer_swap, sizeof(immunity_timer_swap), 1, file);
}