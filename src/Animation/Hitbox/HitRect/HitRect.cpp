#include "./HitRect.hpp"

HitRect::HitRect(float x_offset, float y_offset, float width, float height)
	:Hitbox(x_offset, y_offset){
		this->height = height;
		this->width = width;
		this->type = RECT;
}

HitRect::~HitRect(){}

float HitRect::getHeight(){
	return this->height;
}

float HitRect::getWidth(){
	return this->width;
}

void HitRect::setScale(float x_scale, float y_scale){
	this->curr_x_offset = x_scale * this->base_x_offset;
	this->curr_y_offset = y_scale * this->base_y_offset;
}