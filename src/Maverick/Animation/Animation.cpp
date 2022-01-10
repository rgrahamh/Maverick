#include "./Animation.hpp"

#include "../Engine/Engine.hpp"

extern Engine* engine;

/** The parameterized constructor of the Animation
 * @param x_base A pointer to the int of the base object's X location
 * @param y_base A pointer to the int of the base object's Y location
 */
Animation::Animation(const char* name, double* x_base, double* y_base){
    int name_len = strlen(name);
    this->name = (char*)malloc(name_len + 1);
    memcpy(this->name, name, name_len);
    this->name[name_len] = '\0';

	this->sequence = NULL;
	this->sequence_start = NULL;
	this->sequence_end = NULL;
	this->x_base = x_base;
	this->y_base = y_base;
	this->x_scale = 1.0;
	this->y_scale = 1.0;
	this->time_counter = 0;
	this->paused = false;

	this->next_animation = nullptr;
}

/** Animation destructor
 */
Animation::~Animation(){
	if(sequence_start != nullptr){
		AnimationSeq* cursor = sequence_start;
		do {
			HitboxList* hitboxes = cursor->hitboxes;
			while(hitboxes != NULL){
				if(hitboxes->hitbox != NULL){
					//delete hitboxes->hitbox;
				}

				HitboxList* tmp = hitboxes;
				hitboxes = hitboxes->next;
				delete tmp;
			}

			if(cursor->sprite != nullptr){
				if(cursor->sprite->rect != nullptr){
					free(cursor->sprite->rect);
				}

				if(cursor->sprite->texture != nullptr){
					SDL_DestroyTexture(cursor->sprite->texture);
				}

				delete cursor->sprite;
			}

			AnimationSeq* tmp;
			tmp = cursor;
			cursor = cursor->next;
			free(tmp);
		} while(cursor != sequence_start);
	}

	free(name);
}

/** Gets the current sprite
 * @return The current sprite
 */
Sprite* Animation::getSprite(){
	if(this->sequence){
		return this->sequence->sprite;
	}
	else{
		return nullptr;
	}
}

/** Gets the current hitboxes
 * @return The current hitboxes
 */
HitboxList* Animation::getHitboxes(){
	if(this->sequence){
		return this->sequence->hitboxes;
	}
	else{
		return nullptr;
	}
}

/** Gets the draw axis
 * @return The draw axis
 */
float Animation::getDrawAxis(){
	if(this->sequence != NULL && this->sequence->hitboxes != NULL){
		return this->sequence->hitboxes->hitbox->getBotBound();
	}
	else if(this->sequence != NULL){
		return this->sequence->sprite->rect->y + this->sequence->sprite->curr_y_offset;
	}
	else{
		return 0.0;
	}
}

/** Gets the number of frames left in the animation from the current state
 * @return The number of frames left in the animation
 */
uint32_t Animation::getTimeLeft(){
	AnimationSeq* cursor = this->sequence;
	int time_left = cursor->keytime - this->time_counter;
	while(cursor != this->sequence_start){
		time_left += cursor->keytime;
		cursor = cursor->next;
	}
	return time_left;
}

/** Gets if the animation is paused
 * @return If the animation is paused
 */
bool Animation::getPaused(){
	return this->paused;
}

/** Sets if the animation is paused
 * @param paused If the animation is paused
 */
void Animation::setPaused(bool paused){
	this->paused = paused;
}

/** If the animation is animated
 * @return If the animation is animated (if it has multiple frames)
 */
bool Animation::isAnimated(){
	return (this->sequence_start == this->sequence_end || this->sequence_start == NULL)? false : true;
}

/** Adds a frame to an animation
 * @param sprite_path The filepath of the sprite
 * @param keytime The number of frames before the key continues
 * @param x_offset The X offset of the new sprite
 * @param y_offset The Y offset of the new sprite
 * @param width The width of the new sprite 
 * @param height The height of the new sprite
 */
int Animation::addFrame(const char* sprite_path, unsigned int keytime, double x_offset, double y_offset, int width, int height){
	//Getting the texture
	SDL_Surface* surface;
	if((surface = engine->getSurface(sprite_path)) == NULL){
		if(surface == nullptr){
			printf("Cannot find image %s!\n", sprite_path);
			return -1;
		}
	}

	//Create the rect
	SDL_Rect* rect = new SDL_Rect();
	if(width == -1){
		rect->w = surface->w;
	}
	else{
		rect->w = width;
	}
	if(height == -1){
		rect->h = surface->h;
	}
	else{
		rect->h = height;
	}

	unsigned int sprite_name_len = strlen(sprite_path);
	char* sprite_name = (char*)malloc(sprite_name_len + 1);
	memcpy(sprite_name, sprite_path, sprite_name_len);
	sprite_name[sprite_name_len] = '\0';

	//Create the sprite
	Sprite* sprite = new Sprite();
	sprite->name = sprite_name;
	sprite->rotation = 0.0;

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
	this->sequence_end->sprite->rect = rect;
	this->sequence_end->sprite->surface = surface;
	this->sequence_end->sprite->texture = NULL;
	this->sequence_end->sprite->base_x_offset = x_offset;
	this->sequence_end->sprite->base_y_offset = y_offset;
	this->sequence_end->sprite->curr_x_offset = x_offset;
	this->sequence_end->sprite->curr_y_offset = y_offset;
	this->sequence_end->keytime = keytime;
	this->sequence_end->hitboxes = NULL;

	//Set up the circular link
	this->sequence_end->next = this->sequence_start;

	return 0;
}

/** Adds a new hitbox to an animation (to all sprites)
 * @param hitbox The hitbox to add
 */
int Animation::addHitbox(Hitbox* hitbox){
	AnimationSeq* cursor = this->sequence_start;
	if(cursor != NULL){
		do{
			HitboxList* new_hitbox = new HitboxList;
			new_hitbox->hitbox = hitbox;
			new_hitbox->next = cursor->hitboxes;
			cursor->hitboxes = new_hitbox;

			cursor = cursor->next;
		} while (cursor != this->sequence_start);

		return 0;
	}
	else{
		return -1;
	}
}

/** Adds a new hitbox to an animation (with sprite number)
 * @param hitbox The hitbox to add
 * @param sequence_num The sprite number (-1 adds to the last sprite)
 */
int Animation::addHitbox(Hitbox* hitbox, int sequence_num){
	if(this->sequence_start != nullptr){
		if(sequence_num == -1){
			HitboxList* new_hitbox = new HitboxList;
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
			for(int i = 0; i < sequence_num && cursor != NULL; i++){
				cursor = cursor->next;
			}
			if(cursor != NULL){
				HitboxList* new_hitbox = new HitboxList;
				new_hitbox->hitbox = hitbox;
				new_hitbox->next = cursor->hitboxes;
				cursor->hitboxes = new_hitbox;
			}
		}
		return 0;
	}
	else{
		return -1;
	}
}

/** Adds a new sound to an animation (given the sprite number)
 * @param sound The sound to add
 * @param sequence_num The sequence number (-1 adds to the last element of the sequence)
 */
int Animation::addSound(Sound* sound, int sequence_num){
	if(this->sequence_start != nullptr){
		if(sequence_num == -1){
			if(this->sequence_end != NULL){
				this->sequence_end->sound = sound;
			}
			else{
				return -1;
			}
		}
		else{
			AnimationSeq* cursor = this->sequence_start;
			for(int i = 0; i < sequence_num && cursor != NULL; i++){
				cursor = cursor->next;
			}
			if(cursor != NULL){
				cursor->sound = sound;
			}
		}
		return 0;
	}
	else{
		return -1;
	}
}

/** Sets the next animation
 * @param next_animation The next animation
 */
void Animation::setNextAnimation(Animation* next_animation){
	this->next_animation = next_animation;
}

/** Sets the scale of the animation
 * @param x_scale The X scale factor
 * @param y_scale the Y scale factor
 */
int Animation::setScale(double x_scale, double y_scale){
	if(this->sequence_start != nullptr){
		AnimationSeq* cursor = sequence_start;
		if(cursor != NULL){
			do{
				cursor->sprite->curr_x_offset *= x_scale / this->x_scale;
				cursor->sprite->curr_y_offset *= y_scale / this->y_scale;

				if(cursor->sprite->rect != NULL){
					cursor->sprite->rect->w *= x_scale / this->x_scale;
					cursor->sprite->rect->h *= y_scale / this->y_scale;
				}
				if(cursor->hitboxes != NULL){
					for(HitboxList* hitboxlst = cursor->hitboxes; hitboxlst != NULL; hitboxlst = hitboxlst->next){
						hitboxlst->hitbox->setScale(x_scale, y_scale);
					}
				}

				cursor = cursor->next;
			} while(cursor != sequence_start);
		}

		this->x_scale = x_scale;
		this->y_scale = y_scale;

		return 0;
	}
	else{
		return -1;
	}
}

/** Sets the size of the animation
 * @param x_scale The X size
 * @param y_scale the Y size
 */
int Animation::setSize(int width, int height){
	if(this->sequence_start != nullptr){
		AnimationSeq* cursor = sequence_start;
		if(cursor != NULL){
			do{
				if(cursor->sprite->rect != NULL){
					cursor->sprite->rect->w = width;
					cursor->sprite->rect->h = height;
				}

				cursor = cursor->next;
			} while(cursor != sequence_start);
		}

		return 0;
	}
	else{
		return -1;
	}
}

/** Advances the animation by a frame
 */
void Animation::advance(uint32_t delta){
	if(this->isAnimated() && !this->paused){
		time_counter += delta;
		//If we've exceeded the keytime for this frame
		if(sequence->keytime <= time_counter){
			//If we've hit the end of the sequence & next_animation is not nullptr (for a loop, next_sequence should be start_sequence)
			bool progressed = false;
			if(sequence == sequence_end && next_animation != nullptr){
				sequence = next_animation->getSequenceStart();
				progressed = true;
			}
			//If we haven't hit the end of the sequence
			else if(sequence != sequence_end){
				sequence = sequence->next;
				progressed = true;
			}

			//Play the new sound, if it exists
			if(sequence->sound != nullptr && progressed){
				engine->getSoundBoard()->playSound(sequence->sound->sample, 0);
			}

			//If we're on the last frame of the sequence & next_animation is null, maintain the current frame
			time_counter -= sequence->keytime;
		}
	}
}

/** Starts the animation over again
 */
void Animation::start(){
	sequence = sequence_start;
	time_counter = 0;
}

/** Called for the animation's draw step
 * @param window The current window that is being drawn to
 */
void Animation::draw(SDL_Renderer* renderer, uint32_t delta, float camera_x, float camera_y){
	// Check to see if we've been initialized
	if(this->sequence == NULL){
		return;
	}

	//Advance the animation if not paused
	if(!engine->checkState(GAME_STATE::PAUSE)){
		this->advance(delta);
	}

	Sprite* sprite = this->sequence->sprite;
	if(sprite == nullptr){
		return;
	}

	//Update the sprite position
	SDL_Rect* curr_rect = sprite->rect;
	curr_rect->x = *this->x_base;
	curr_rect->y = *this->y_base;

	if(sprite->texture == NULL && sprite->surface != NULL){
		sprite->texture = SDL_CreateTextureFromSurface(renderer, sprite->surface);
	}

	SDL_Rect draw_rect = *curr_rect;
	draw_rect.x -= camera_x;
	draw_rect.y -= camera_y;

	//Draw the sprite
	if(sprite->rotation){
		if(SDL_RenderCopyEx(renderer, sprite->texture, NULL, &draw_rect, sprite->rotation, NULL, SDL_RendererFlip::SDL_FLIP_NONE)){
			printf(SDL_GetError());
		}
	}
	else{
		if(SDL_RenderCopy(renderer, sprite->texture, NULL, &draw_rect)){
			printf(SDL_GetError());
		}
	}
}

/** Rotates the hitbox slightly to the right (if direction is 0) or left (if direction is 1)
 * @param direction The direction that things are being rotated to
 * @param rotate_dist The distance you should rotate
 */
void Animation::rotate(int direction, float rotate_amnt){
	AnimationSeq* animations = sequence_start;
	while(animations != NULL){
		animations->sprite->rotation += rotate_amnt * ((direction)? -1 : 1);

		HitboxList* hitboxes = animations->hitboxes;
		while(hitboxes != NULL){
			Hitbox* hitbox = hitboxes->hitbox;
			if(hitbox->getShape() == CONE){
				((HitCone*)hitbox)->rotate(direction, rotate_amnt);
				hitboxes = hitboxes->next;
			}
		}
	}
}

/** Gets the start of the animation sequence
 * @return The start of the animation sequence
 */
AnimationSeq* Animation::getSequenceStart(){
	return this->sequence_start;
}

/** Gets the end of the animation sequence
 * @return The end of the animation sequence
 */
AnimationSeq* Animation::getSequenceEnd(){
	return this->sequence_end;
}

/** Gets the name of the animation
 * @return The name of the animation
 */
const char* Animation::getName(){
	return this->name;
}