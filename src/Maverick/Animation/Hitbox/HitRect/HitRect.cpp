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
HitRect::HitRect(double* x_base, double* y_base, double x_offset, double y_offset, float width, float height, unsigned int type, int32_t hitbox_group, uint32_t immunity_timer)
	:Hitbox(x_base, y_base, x_offset, y_offset, type, hitbox_group, immunity_timer){
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
void HitRect::setScale(double x_scale, double y_scale){
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

/** Gets the top hitbox bound
 * @return The top hitbox bound
 */
float HitRect::getTopBound(){
	return *this->y_base + this->y_curr_offset;
}

/** Gets the bottom hitbox bound
 * @return The bottom hitbox bound
 */
float HitRect::getBotBound(){
	return *this->y_base + this->y_curr_offset + this->curr_height;
}

/** Gets the left hitbox bound
 * @return The left hitbox bound
 */
float HitRect::getLeftBound(){
	return *this->x_base + this->x_curr_offset;
}

/** Gets the right hitbox bound
 * @return The right hitbox bound
 */
float HitRect::getRightBound(){
	return *this->x_base + this->x_curr_offset + this->curr_width;
}

/** Gets the draw axis of the hitbox
 * @return The draw axis of the hitbox
 */
float HitRect::getDrawAxis(){
	return *this->y_base + this->y_curr_offset + (this->curr_height / 2);
}

/** Serialize the HitRect
 * @param file The file that serialized data is outputted to
 */
void HitRect::serializeData(FILE* file){
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
	uint64_t width_swap = EndianSwap((uint64_t*)&this->base_width);
	fwrite(&width_swap, sizeof(width_swap), 1, file);

	//Write Y component (Y radius/height)
	uint64_t height_swap = EndianSwap((uint64_t*)&this->base_height);
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