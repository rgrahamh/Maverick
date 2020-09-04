#include "./Animation.hpp"

/** The parameterized constructor of the Animation
 * @param x_base A pointer to the int of the base object's X location
 * @param y_base A pointer to the int of the base object's Y location
 * @param animated If the animation progresses
 */
Animation::Animation(float* x_base, float* y_base, bool animated = true){
	this->sequence = NULL;
	this->sequence_start = NULL;
	this->sequence_end = NULL;
	this->x_base = x_base;
	this->y_base = y_base;
	this->frame_counter = 0;
	this->animated = animated;
}

/** Animation destructor
 */
Animation::~Animation(){
	SpriteLst* tmp;
	while(sequence_start != NULL){
		tmp = sequence_start->next;
		free(sequence_start);
		sequence_start = tmp;
	}
}

/** Adds a frame to an animation
 * @param sprite_path The filepath of the sprite
 * @param keyframe The number of frames before the key continues
 * @param x_offset The X offset of the new sprite
 * @param y_offset The Y offset of the new sprite
 */
void Animation::addFrame(const char* sprite_path, unsigned int keyframe, float x_offset, float y_offset){
	//Getting the sprite
	sf::Sprite* sprite;
	if((sprite = sprite_hash->get(sprite_path)) == NULL){
		sf::Texture* texture = new sf::Texture();
		if(!texture->loadFromFile(sprite_path)){
			printf("Cannot find image %s!\n", sprite_path);
			return;
		}
		sprite = new sf::Sprite(*texture);

		sprite_hash->add(sprite_path, sprite);
	}

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
	//Regardless
	this->sequence_end->sprite = sprite;
	this->sequence_end->keyframe = keyframe;
	this->sequence_end->x_offset = x_offset;
	this->sequence_end->y_offset = y_offset;

	//Set up the circular link
	this->sequence_end->next = this->sequence_start;
}

/** Sets the scale of the animation
 * @param x_scale The X scale factor
 * @param y_scale the Y scale factor
 */
void Animation::setScale(float x_scale, float y_scale){
	SpriteLst* cursor = sequence_start;
	do{
		cursor->sprite->setScale(x_scale, y_scale);
		cursor = cursor->next;
	} while(cursor != sequence_end);
}

/** Advances the animation by a frame
 */
void Animation::advance(){
	if(animated && sequence->keyframe == frame_counter++){
		sequence = sequence->next;
		frame_counter = 0;
	}
}

/** Starts the animation over again
 */
void Animation::start(){
	sequence = sequence_start;
	frame_counter = 0;
}

/** Gets the current sprite
 */
sf::Sprite* Animation::getSprite(){
	return this->sequence->sprite;
}
