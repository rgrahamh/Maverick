Animation::Animation(ImgSeq* sequence, float* x, float* y, bool animated = true){
	this->sequence = sequence;
	this->start = sequence;
	this->x_orig = x;
	this->y_orig = y;
	this->frame_counter = 0;
	this->animated = animated;
}

void Animation::advance(){
	if(animated && sequence->frame_len == frame_counter++){
		sequence = sequence->next;
		frame_counter = 0;
	}
}

void Animation::start(){
	sequence = start;
	frame_counter = 0;
}

void Animation::draw(){
	//Draw the image
	//sf::draw((*x_orig) + sequence->x_offset, (*x_orig) + sequence->y_offset)
}
