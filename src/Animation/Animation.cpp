#include "./Animation.hpp"

Animation::Animation(float* x_base, float* y_base, bool animated = true){
	this->sequence = NULL;
	this->sequence_start = NULL;
	this->x_base = x_base;
	this->y_base = y_base;
	this->frame_counter = 0;
	this->animated = animated;
}

//MAKE SURE TO IMPLEMENT THIS AS CIRCULARLY LINKED
void Animation::addFrame(sf::Sprite* sprite, unsigned int keyframe, float x_offset, float y_offset){
	//If it's the first animation frame
	if(this->sequence_end == NULL){
		this->sequence_end = new SpriteLst;
		this->sequence_start = this->sequence_end;
		this->sequence = this->sequence_end;
	}
	//Any additional frames
	else{
		this->sequence_end->next = new SpriteLst;
		this->sequence_end = this->sequence_end->next;
	}
	this->sequence_end->sprite = sprite;
	this->sequence_end->keyframe = keyframe;
	this->sequence_end->x_offset = x_offset;
	this->sequence_end->y_offset = y_offset;

	//Set up the circular link
	this->sequence_end->next = this->sequence_start;
}

void Animation::advance(){
	if(animated && sequence->keyframe == frame_counter++){
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
