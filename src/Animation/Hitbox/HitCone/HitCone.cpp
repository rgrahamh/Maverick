#include "./HitCone.hpp"

HitCone::HitCone(float x_offset, float y_offset, float radius, float angle, float slice_prop)
	:Hitbox(x_offset, y_offset){
	this->base_radius = radius;
	this->curr_radius = radius;
	this->angle = angle;
	this->slice_prop = slice_prop;
	this->type = CONE;
}

HitCone::~HitCone(){}

void HitCone::setScale(float x_scale, float y_scale){
	this->curr_x_offset = x_scale * this->base_x_offset;
	this->curr_y_offset = y_scale * this->base_y_offset;
	this->curr_radius = ((x_scale + y_scale) / 2.0f) * this->base_radius;
}

float HitCone::getAngle(){
	return this->angle;
}

float HitCone::getProp(){
	return this->slice_prop;
}

float HitCone::getRadius(){
	return this->curr_radius;
}

//Direction of 0 is left, direction of 1 is right
void HitCone::rotate(int direction){
	if(direction == 0){
		this->angle -= 0.01;
	}
	else{
		this->angle += 0.01;
	}
}