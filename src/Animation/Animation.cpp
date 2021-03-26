#include "./Animation.hpp"

/** The parameterized constructor of the Animation
 * @param x_base A pointer to the int of the base object's X location
 * @param y_base A pointer to the int of the base object's Y location
 */
Animation::Animation(float* x_base, float* y_base, unsigned char draw_layer){
	this->sequence = NULL;
	this->sequence_start = NULL;
	this->sequence_end = NULL;
	this->x_base = x_base;
	this->y_base = y_base;
	this->frame_counter = 0;
	this->draw_layer = draw_layer;
}

/** Animation destructor
 */
Animation::~Animation(){
	/*while(sequence_start != NULL){
		HitboxLst* hitboxes = sequence_start->hitboxes;
		while(hitboxes != NULL){
			if(hitboxes->hitbox != NULL){
				delete hitboxes->hitbox;
			}

			HitboxLst* tmp = hitboxes;
			hitboxes = hitboxes->next;
			delete hitboxes;
		}

		delete sequence_start->sprite;

		AnimationSeq* tmp;
		tmp = sequence_start;
		sequence_start = sequence_start->next;
		free(tmp);
	}*/
}

/** Gets the current sprite
 * @return The current sprite
 */
sf::Sprite* Animation::getSprite(){
	return this->sequence->sprite;
}

/** Gets the current hitboxes
 * @return The current hitboxes
 */
HitboxLst* Animation::getHitboxes(){
	return this->sequence->hitboxes;
}

/** Gets the current draw layer
 * @return The current draw layer
 */
unsigned char Animation::getDrawLayer(){
	return this->draw_layer;
}

/** Gets the draw axis
 * @return The draw axis
 */
float Animation::getDrawAxis(){
	if(this->sequence->hitboxes != NULL){
		return this->sequence->hitboxes->hitbox->getBotBound();
	}
	else{
		return this->sequence->sprite->getTexture()->getSize().y + this->sequence->curr_y_offset;
	}
}

/** Gets the number of frames left in the animation from the current state
 * @return The number of frames left in the animation
 */
int Animation::getFramesLeft(){
	AnimationSeq* cursor = this->sequence;
	int frames_left = cursor->keyframe - this->frame_counter;
	while(cursor != this->sequence_start){
		frames_left += cursor->keyframe;
		cursor = cursor->next;
	}
	return frames_left;
}

/** If the animation is animated
 * @return If the animation is animated (if it has multiple frames)
 */
bool Animation::isAnimated(){
	return (this->sequence_start == this->sequence_end)? false : true;
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
	this->sequence_end->base_x_offset = x_offset;
	this->sequence_end->base_y_offset = y_offset;
	this->sequence_end->curr_x_offset = x_offset;
	this->sequence_end->curr_y_offset = y_offset;
	this->sequence_end->hitboxes = NULL;

	//Set up the circular link
	this->sequence_end->next = this->sequence_start;
}

/** Adds a new hitbox to an animation (to all sprites)
 * @param hitbox The hitbox to add
 */
void Animation::addHitbox(Hitbox* hitbox){
	AnimationSeq* cursor = this->sequence_start;
	if(cursor != NULL){
		do{
			HitboxLst* new_hitbox = new HitboxLst;
			new_hitbox->hitbox = hitbox;
			new_hitbox->next = cursor->hitboxes;
			cursor->hitboxes = new_hitbox;

			cursor = cursor->next;
		} while (cursor != this->sequence_start);
	}
}

/** Adds a new hitbox to an animation (with sprite number)
 * @param hitbox The hitbox to add
 * @param sprite_num The sprite number (-1 adds to the last sprite)
 */
void Animation::addHitbox(Hitbox* hitbox, int sprite_num){
	if(sprite_num == -1){
		HitboxLst* new_hitbox = new HitboxLst;
		new_hitbox->hitbox = hitbox;
		new_hitbox->next = NULL;
		if(this->sequence_end != NULL){
			if(this->sequence_end->hitboxes == NULL){
				this->sequence_end->hitboxes = new_hitbox;
			}
			else{
				this->sequence_end->hitboxes->next = new_hitbox;
			}
		}
	}
	else{
		AnimationSeq* cursor = this->sequence_start;
		for(int i = 0; i < sprite_num && cursor != NULL; i++){
			cursor = cursor->next;
		}
		if(cursor != NULL){
			HitboxLst* new_hitbox = new HitboxLst;
			new_hitbox->hitbox = hitbox;
			new_hitbox->next = cursor->hitboxes;
			cursor->hitboxes = new_hitbox;
		}
	}
}

/** Sets the scale of the animation
 * @param x_scale The X scale factor
 * @param y_scale the Y scale factor
 */
void Animation::setScale(float x_scale, float y_scale){
	AnimationSeq* cursor = sequence_start;
	if(cursor != NULL){
		do{
			cursor->curr_x_offset = cursor->base_x_offset * x_scale;
			cursor->curr_y_offset = cursor->base_y_offset * y_scale;

			if(cursor->sprite != NULL){
				cursor->sprite->setScale(x_scale, y_scale);
			}
			if(cursor->hitboxes != NULL){
				for(HitboxLst* hitboxlst = cursor->hitboxes; hitboxlst != NULL; hitboxlst = hitboxlst->next){
					hitboxlst->hitbox->setScale(x_scale, y_scale);
				}
			}

			cursor = cursor->next;
		} while(cursor != sequence_start);
	}
}

/** Advances the animation by a frame
 */
void Animation::advance(){
	if(this->isAnimated() && sequence->keyframe == frame_counter++){
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

/** Called for the animation's draw step
 * @param window The current window that is being drawn to
 */
void Animation::draw(sf::RenderWindow* window){
	//Advance the animation
	this->advance();

	//Update the sprite position
	sf::Sprite* curr_sprite = this->sequence->sprite;
	curr_sprite->setPosition(*this->x_base + this->sequence->curr_x_offset, *this->y_base + this->sequence->curr_y_offset);

	//Draw the sprite
	window->draw(*curr_sprite);
}

/** Rotates the hitbox slightly to the right (if direction is 0) or left (if direction is 1)
 * @param direction The direction that things are being rotated to
 * @param rotate_dist The distance you should rotate
 */
void Animation::rotate(int direction, float rotate_amnt){
	AnimationSeq* animations = sequence_start;
	while(animations != NULL){
		animations->sprite->rotate(rotate_amnt * ((direction)? -1 : 1));

		HitboxLst* hitboxes = animations->hitboxes;
		while(hitboxes != NULL){
			Hitbox* hitbox = hitboxes->hitbox;
			if(hitbox->getShape() == CONE){
				((HitCone*)hitbox)->rotate(direction, rotate_amnt);
				hitboxes = hitboxes->next;
			}
		}
	}
}
