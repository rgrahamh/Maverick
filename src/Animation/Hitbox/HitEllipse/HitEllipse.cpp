#include "./HitEllipse.hpp"

HitEllipse::HitEllipse(float* x_base, float* y_base, float x_offset, float y_offset, float x_radius, float y_radius)
	:Hitbox(x_base, y_base, x_offset, y_offset){
	this->x_base_radius = x_radius;
	this->y_base_radius = y_radius;
	this->x_curr_radius = x_radius;
	this->y_curr_radius = y_radius;
	this->type = ELLIPSE;
}

HitEllipse::~HitEllipse(){}

float HitEllipse::getXRadius(){
	return this->x_curr_radius;
}

float HitEllipse::getYRadius(){
	return this->y_curr_radius;
}

bool HitEllipse::checkCollision(Hitbox* other){
	switch (other->getType()){
		case RECT:
			break;

		case ELLIPSE:
			break;

		case CONE:
			break;
	}
	return false;
}

void HitEllipse::setScale(float x_scale, float y_scale){
	this->x_curr_offset = x_scale * this->x_base_offset;
	this->y_curr_offset = y_scale * this->y_base_offset;
	this->x_curr_radius = (x_scale / 2.0f) * this->x_base_radius;
	this->y_curr_radius = (x_scale / 2.0f) * this->y_base_radius;
}