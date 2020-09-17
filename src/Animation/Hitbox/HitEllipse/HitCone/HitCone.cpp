#include "./HitCone.hpp"

HitCone::HitCone(float* x_base, float* y_base, float x_offset, float y_offset, float x_radius, float y_radius, float angle, float slice_prop)
	:HitEllipse(x_base, y_base, x_offset, y_offset, x_radius, y_radius){
	this->angle = angle;
	this->slice_prop = slice_prop;
	this->type = CONE;
}

HitCone::~HitCone(){}

void HitCone::setScale(float x_scale, float y_scale){
	this->x_curr_offset = x_scale * this->x_base_offset;
	this->y_curr_offset = y_scale * this->y_base_offset;
}

float HitCone::getAngle(){
	return this->angle;
}

float HitCone::getProp(){
	return this->slice_prop;
}

//Direction of 0 is left, direction of 1 is right
void HitCone::rotate(int direction){
	if(direction == 0){
		this->angle -= 1.5;
	}
	else{
		this->angle += 1.5;
	}
}

bool HitCone::checkCollision(Hitbox* other){
	return false;
}