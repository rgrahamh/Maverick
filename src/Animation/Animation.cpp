#include "./Animation.hpp"

Animation::Animation(ImgSeq* sequence, float* x, float* y, bool animated = true){
	this->sequence = sequence;
	this->sequence_start = sequence;
	this->x_orig = x;
	this->y_orig = y;
	this->frame_counter = 0;
	this->animated = animated;
}

void Animation::advance(){
	if(animated && sequence->step_len == frame_counter++){
		sequence = sequence->next;
		frame_counter = 0;
	}
}

void Animation::start(){
	sequence = sequence_start;
	frame_counter = 0;
}

sf::Sprite* Animation::getSprite(){
	return this->sequence->sprite;
}
