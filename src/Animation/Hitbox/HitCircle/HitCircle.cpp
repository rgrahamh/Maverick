#include "./HitCircle.hpp"

HitCircle::HitCircle(float x_offset, float y_offset, float radius)
	:Hitbox(x_offset, y_offset){
	this->base_radius = radius;
	this->curr_radius = radius;
	this->type = CIRCLE;
}

HitCircle::~HitCircle(){}

float HitCircle::getRadius(){
	return this->curr_radius;
}

void HitCircle::setScale(float x_scale, float y_scale){
	this->curr_x_offset = x_scale * this->base_x_offset;
	this->curr_y_offset = y_scale * this->base_y_offset;
	this->curr_radius = ((x_scale + y_scale) / 2.0f) * this->base_radius;
}