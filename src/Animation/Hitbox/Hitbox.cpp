#include "./Hitbox.hpp"

Hitbox::Hitbox(float* x_base, float* y_base, float x_offset, float y_offset){
	this->x_base = x_base;
	this->y_base = y_base;
	this->x_base_offset = x_offset;
	this->y_base_offset = y_offset;
	this->x_curr_offset = x_offset;
	this->y_curr_offset = y_offset;
}

Hitbox::~Hitbox(){}

float Hitbox::getX(){
	return *this->x_base + this->x_curr_offset;
}

float Hitbox::getY(){
	return *this->y_base + this->y_curr_offset;
}

HITBOX_TYPE Hitbox::getType(){
	return this->type;
}

void Hitbox::setScale(float x_scale, float y_scale){
	this->x_curr_offset = x_scale * x_base_offset;
	this->y_curr_offset = y_scale * y_base_offset;
}

bool Hitbox::isPointInside(float x_coord, float y_coord){
	return false;
}