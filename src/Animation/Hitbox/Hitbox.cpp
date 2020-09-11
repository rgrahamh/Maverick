#include "./Hitbox.hpp"

Hitbox::Hitbox(float x_offset, float y_offset){
	this->base_x_offset = x_offset;
	this->base_y_offset = y_offset;
	this->curr_x_offset = x_offset;
	this->curr_y_offset = y_offset;
}

Hitbox::~Hitbox(){}

void Hitbox::setScale(float x_scale, float y_scale){
	this->curr_x_offset = x_scale * base_x_offset;
	this->curr_y_offset = y_scale * base_y_offset;
}