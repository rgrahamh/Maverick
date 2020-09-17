#include "./Animation.hpp"

/** The parameterized constructor of the Animation
 * @param x_base A pointer to the int of the base object's X location
 * @param y_base A pointer to the int of the base object's Y location
 * @param animated If the animation progresses
 */
Animation::Animation(float* x_base, float* y_base, bool animated){
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
	AnimationSeq* tmp;
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
	//Getting the texture
	sf::Texture* texture;
	if((texture = texture_hash->get(sprite_path)) == NULL){
		texture = new sf::Texture();
		if(!texture->loadFromFile(sprite_path)){
			printf("Cannot find image %s!\n", sprite_path);
			return;
		}

		texture_hash->add(sprite_path, texture);
	}
	sf::Sprite* sprite;
	sprite = new sf::Sprite(*texture);

	//If it's the first animation frame
	if(this->sequence_end == NULL){
		this->sequence_end = new AnimationSeq;
		this->sequence_start = this->sequence_end;
		this->sequence = this->sequence_end;
	}
	//Any additional frames
	else{
		this->sequence_end->next = new AnimationSeq;
		this->sequence_end = this->sequence_end->next;
		this->sequence_end->next = this->sequence_start;
	}
	//Regardless
	this->sequence_end->sprite = sprite;
	this->sequence_end->keyframe = keyframe;
	this->sequence_end->x_offset = x_offset;
	this->sequence_end->y_offset = y_offset;

	//Set up the circular link
	this->sequence_end->next = this->sequence_start;
}

/** Adds a new hitbox to an animation (with an optional sprite number)
 * @param hitbox The hitbox to add
 * @param sprite_num The sprite number (default -1 adds to the last sprite)
 */
void Animation::addHitbox(Hitbox* hitbox, int sprite_num){
	if(sprite_num == -1){
		HitboxLst* new_hitbox = new HitboxLst();
		new_hitbox->hitbox = hitbox;
		new_hitbox->next = NULL;
		if(this->sequence_end != NULL){
			if(sequence_end->hitboxes == NULL){
				this->sequence_end->hitboxes = new_hitbox;
			}
			else{
				this->sequence_end->hitboxes->next = new_hitbox;
			}
		}
	}
	else{
		HitboxLst* cursor = this->sequence_start->hitboxes;
		for(int i = 0; i < sprite_num && cursor != NULL; i++){
			cursor = cursor->next;
		}
		if(cursor != NULL){
			HitboxLst* new_hitbox = new HitboxLst();
			new_hitbox->hitbox = hitbox;
			new_hitbox->next = NULL;
			cursor->next = new_hitbox;
		}
	}
}

/** Sets the scale of the animation
 * @param x_scale The X scale factor
 * @param y_scale the Y scale factor
 */
void Animation::setScale(float x_scale, float y_scale){
	AnimationSeq* cursor = sequence_start;
	do{
		cursor->sprite->setScale(x_scale, y_scale);
		cursor->hitboxes->hitbox->setScale(x_scale, y_scale);
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

void Animation::draw(sf::RenderWindow* window){
	//Advance the animation
	this->advance();

	//Update the sprite position
	sf::Sprite* curr_sprite = this->sequence->sprite;
	curr_sprite->setPosition(*this->x_base + this->sequence->x_offset, *this->y_base + this->sequence->y_offset);

	//Draw the sprite
	window->draw(*curr_sprite);
}
