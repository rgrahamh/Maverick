#include "./HitRect.hpp"

HitRect::HitRect(float* x_base, float* y_base, float x_offset, float y_offset, float width, float height)
	:Hitbox(x_base, y_base, x_offset, y_offset){
		this->base_height = height;
		this->curr_height = height;
		this->base_width = width;
		this->curr_width = width;
		this->type = RECT;
}

HitRect::~HitRect(){}

float HitRect::getHeight(){
	return this->curr_height;
}

float HitRect::getWidth(){
	return this->curr_width;
}

bool HitRect::isPointInside(float x_coord, float y_coord){
	if(x_coord > *this->x_base + this->x_curr_offset
	&& x_coord < *this->x_base + this->x_curr_offset + this->curr_width
	&& y_coord > *this->y_base + this->y_curr_offset
	&& y_coord < *this->y_base + this->y_curr_offset + this->curr_height){
		return true;
	}
	return false;
}

void HitRect::setScale(float x_scale, float y_scale){
	this->x_curr_offset = x_scale * this->x_base_offset;
	this->y_curr_offset = y_scale * this->y_base_offset;
	this->curr_height = y_scale * this->base_height ;
	this->curr_width = y_scale * this->base_width;
}

bool HitRect::checkCollision(Hitbox* other){
	switch (other->getType()){
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