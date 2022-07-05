#include "./Animation.hpp"

#include "../Engine/Engine.hpp"
#include "../Global/Global.hpp"

extern Engine* engine;
extern bool debug;

//FINISH MULTIPLE SPRITES

/** The parameterized constructor of the Animation
 * @param name The name of the animation
 * @param x_base A pointer to the int of the base object's X location
 * @param y_base A pointer to the int of the base object's Y location
 * @param num_sprite_sets The max number of sprite sets that should be allocated for this animation
 */
Animation::Animation(const char* name, double* x_base, double* y_base, uint16_t num_sprite_sets){
    this->name = StrDeepCopy(name);
	this->sequence = NULL;
	this->sequence_start = NULL;
	this->sequence_end = NULL;
	this->x_base = x_base;
	this->y_base = y_base;
	this->x_scale = 1.0;
	this->y_scale = 1.0;
	this->time_counter = 0;
	this->paused = false;
	this->num_sprite_sets = num_sprite_sets;
	this->sprite_set_counter = 0;
	this->curr_sprite_set = 0;

	this->next_animation = nullptr;
}

Animation::Animation(FILE* file){
	this->deserializeData(file);
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

			for(auto& sprite_set:this->sprite_sets){
				if(cursor->sprite != nullptr){
					if(cursor->sprite[sprite_set.second])
					{
						if(cursor->sprite[sprite_set.second]->rect != nullptr){
							delete cursor->sprite[sprite_set.second]->rect;
						}

						if(cursor->sprite[sprite_set.second]->texture != nullptr){
							SDL_DestroyTexture(cursor->sprite[sprite_set.second]->texture);
						}
					}
				}

				delete cursor->sprite[sprite_set.second];
			}

			if(cursor->sprite != nullptr){
				delete cursor->sprite;
			}

			AnimationSeq* tmp;
			tmp = cursor;
			cursor = cursor->next;
			delete tmp;
		} while(cursor != sequence_start && cursor != nullptr);
	}

	free(name);
}

/** Gets the current sprite
 * @return The current sprite
 */
Sprite* Animation::getSprite(){
	if(this->sequence != nullptr){
		return this->sequence->sprite[curr_sprite_set];
	}
	else{
		return nullptr;
	}
}

/** Gets the current sound
 * @return The current sound
 */
Sound* Animation::getSound(){
	if(this->sequence != nullptr){
		return this->sequence->sound;
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

/** Gets the upper draw axis
 * @return The upper draw axis
 */
double Animation::getUpperDrawAxis(){
	if(this->sequence != NULL && this->sequence->sprite[curr_sprite_set] != NULL){
		//If draw_axis is set
		if(this->sequence->sprite[curr_sprite_set]->upper_draw_axis != -1){
			return *this->y_base + this->sequence->sprite[curr_sprite_set]->upper_draw_axis + this->sequence->sprite[curr_sprite_set]->curr_y_offset;
		}
		//Otherwise, interpolate from the sequence
		else{
			return *this->y_base + this->sequence->sprite[curr_sprite_set]->curr_y_offset;
		}
	}
	//If we can't find the draw axis any other way
	else{
		return 0.0;
	}
}

/** Gets the lower draw axis
 * @return The lower draw axis
 */
double Animation::getLowerDrawAxis(){
	if(this->sequence != NULL && this->sequence->sprite[curr_sprite_set] != NULL){
		//If draw_axis is set
		if(this->sequence->sprite[curr_sprite_set]->lower_draw_axis != -1){
			return *this->y_base + this->sequence->sprite[curr_sprite_set]->lower_draw_axis + this->sequence->sprite[curr_sprite_set]->curr_y_offset;
		}
		//Otherwise, interpolate from the sequence
		else{
			return *this->y_base + this->sequence->sprite[curr_sprite_set]->rect->h + this->sequence->sprite[curr_sprite_set]->curr_y_offset;
		}
	}
	//If we can't find the draw axis any other way
	else{
		return 0.0;
	}
}

/** Gets the number of frames left in the animation from the current state
 * @return The number of frames left in the animation
 */
uint32_t Animation::getTimeLeft(){
	if(this->sequence == nullptr){
		return 0;
	}

	AnimationSeq* cursor = this->sequence;
	int time_left = cursor->keytime;
	cursor = cursor->next;
	while(cursor != this->sequence_start && cursor != nullptr){
		time_left += cursor->keytime;
		cursor = cursor->next;
	}
	return time_left - this->time_counter;
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
 * @return If the animation is animated (if it has multiple frames, and hasn't hit the end of the animation)
 */
bool Animation::isAnimated(){
	return !(this->sequence_start == this->sequence_end ||
			 this->sequence_start == NULL ||
			 (sequence == sequence_end && next_animation == nullptr));
}

/** Adds a frame to an animation
 * @param keytime The number of frames before the key continues
 */
int Animation::addFrame(unsigned int keytime){
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
	this->sequence_end->sprite = (Sprite**)calloc(num_sprite_sets, sizeof(Sprite*));
	this->sequence_end->keytime = keytime;
	this->sequence_end->hitboxes = NULL;

	//Set up the circular link
	this->sequence_end->next = this->sequence_start;

	this->sequence_len++; 

	return 0;
}

/** Adds a sprite to the animation
 * @param sprite_set The sprite set you'd like to add the sprite to
 * @param sprite_name The identifier of the sprite
 * @param x_offset The X offset of the new sprite
 * @param y_offset The Y offset of the new sprite
 * @param width The width of the new sprite 
 * @param height The height of the new sprite
 */
int Animation::addSprite(const char* sprite_set, const char* sprite_path, double x_offset, double y_offset, int width, int height){
	if(this->sprite_sets.find(std::string(sprite_set)) == this->sprite_sets.end()){
		if(this->addSpriteSet(sprite_set) != 0){
			return -1;
		}
	}
	uint16_t sprite_set_idx = this->sprite_sets[std::string(sprite_set)];

	AnimationSeq* sequence_cursor = this->sequence_start;
	if(sequence_cursor != nullptr){
		do{
			if(sequence_cursor->sprite[sprite_set_idx] == nullptr){
				break;
			}
			sequence_cursor = sequence_cursor->next;
		} while(sequence_cursor != sequence_start && sequence_cursor != nullptr);
	}

	if(sequence_cursor == nullptr){
		return -1;
	}

	//Create the sprite
	Sprite* sprite = new Sprite();
	sprite->name = StrDeepCopy(sprite_path);
	sprite->rotation = 0.0;
	sprite->lower_draw_axis = -1.0;
	sprite->upper_draw_axis = -1.0;

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

	sprite->rect = rect;
	sprite->base_x_offset = x_offset;
	sprite->base_y_offset = y_offset;
	sprite->curr_x_offset = x_offset;
	sprite->curr_y_offset = y_offset;
	sprite->surface = surface;
	sprite->texture = nullptr;

	sequence_cursor->sprite[sprite_set_idx] = sprite;

	return 0;
}

/** Adds a new sprite set to an animation
 * @param sprite_set The name of the sprite set you'd like to add
 * @return 0 if successful, -1 otherwise
 */
int Animation::addSpriteSet(const char* sprite_set){
	if(sprite_set_counter < num_sprite_sets && this->sprite_sets.find(std::string(sprite_set)) == this->sprite_sets.end()){
		this->sprite_sets[std::string(sprite_set)] = sprite_set_counter++;
		return 0;
	}
	return -1;
}

/** Adds a new hitbox to an animation (to all sprites)
 * @param hitbox The hitbox to add
 * @return 0 if successful, -1 otherwise
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
 * @return 0 if successful, -1 otherwise
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
 * @param sound_id The sound to add
 * @param sequence_num The sequence number (-1 adds to the last element of the sequence)
 * @return 0 if successful, -1 otherwise
 */
int Animation::addSound(const char* sound_id, int sequence_num){
	if(sound_id == nullptr){
		return -1;
	}

	Sound* sound = engine->getSound(sound_id);
	if(sound == nullptr){
		return -1;
	}

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

/** Sets the size of the animation
 * @param x_scale The X size
 * @param y_scale the Y size
 * @return 0 if successful, -1 otherwise
 */
int Animation::setSize(int width, int height){
	if(this->sequence_start == nullptr){
		return -1;
	}

	AnimationSeq* cursor = this->sequence_start;
	if(cursor != NULL){
		do{
			for(auto& sprite_set:this->sprite_sets){
				if(cursor->sprite[sprite_set.second]->rect != NULL){
					cursor->sprite[sprite_set.second]->rect->w = width;
					cursor->sprite[sprite_set.second]->rect->h = height;
				}
			}

			cursor = cursor->next;
		} while(cursor != this->sequence_start && cursor != nullptr);
	}

	return 0;
}

/** Sets the currently used sprite set
 * @param sprite_set The name of the sprite set
 * @return 0 if the sprite set was set successfully, -1 otherwise (means it couldn't be found)
 */
int Animation::setSpriteSet(const char* sprite_set){
	if(this->sprite_sets.find(std::string(sprite_set)) != this->sprite_sets.end()){
		this->curr_sprite_set = sprite_sets[std::string(sprite_set)];
		return 0;
	}
	return -1;
}

/** The upper draw axis offset, in pixels (from the top of the animation).
 * @param upper_draw_axis The upper draw axis, offset from the top of the animation. -1 assumes position based off of the img size
 * @param sprite_num The sprite number; -1 applies to all sprites
 */
void Animation::setUpperDrawAxis(double upper_draw_axis, int32_t sprite_num){
	AnimationSeq* cursor = this->sequence_start;
	if(sprite_num == -1){
		if(cursor != nullptr){
			do{
				for(int i = 0; i < num_sprite_sets; i++){
					if(cursor->sprite[i] != nullptr){
						cursor->sprite[i]->upper_draw_axis = upper_draw_axis;
					}
				}
				cursor = cursor->next;
			} while(cursor != nullptr && cursor != this->sequence_start);
		}
	}
	else{
		//Get to the sprite_num-th sprite
		for(int i = 0; i < sprite_num && cursor != nullptr; i++){
			cursor = cursor->next;
		}

		//Set the val if it exists
		if(cursor != nullptr){
			for(int i = 0; i < num_sprite_sets; i++){
				cursor->sprite[i]->upper_draw_axis = upper_draw_axis;
			}
		}
	}
}

/** The lower draw axis offset, in pixels (from the top of the animation).
 * @param offset The lower draw axis, offset from the top left of the animation. -1 assumes position based off of the img size
 * @param sprite_num The sprite number; -1 applies to all sprites
 */
void Animation::setLowerDrawAxis(double lower_draw_axis, int32_t sprite_num){
	AnimationSeq* cursor = this->sequence_start;
	if(sprite_num == -1){
		if(cursor != nullptr){
			do{
				for(int i = 0; i < num_sprite_sets; i++){
					if(cursor->sprite[i] != nullptr){
						cursor->sprite[i]->lower_draw_axis = lower_draw_axis;
					}
				}
				cursor = cursor->next;
			} while(cursor != nullptr && cursor != this->sequence_start);
		}
	}
	else{
		//Get to the sprite_num-th sprite
		for(int i = 0; i < sprite_num && cursor != nullptr; i++){
			cursor = cursor->next;
		}

		//Set the val if it exists
		if(cursor != nullptr){
			for(int i = 0; i < num_sprite_sets; i++){
				cursor->sprite[i]->lower_draw_axis = lower_draw_axis;
			}
		}
	}
}

/** Advances the animation by a delta
 * @param delta The number of ms that have passed since the last call to advance()
 */
void Animation::advance(uint64_t delta){
	if(this->isAnimated() && !this->paused){
		time_counter += delta;
		//If we've exceeded the keytime for this frame
		while(sequence->keytime <= time_counter){
			time_counter -= sequence->keytime;

			//If we haven't hit the end of the sequence
			if(sequence != sequence_end){
				sequence = sequence->next;
			}
			//If we've hit the end of the sequence & next_animation is not nullptr (for a loop, next_sequence should be start_sequence)
			else if(sequence == sequence_end && next_animation != nullptr){
				sequence = next_animation->getSequenceStart();
			}
			//If we've hit the end of the sequence & next_animation is nullptr
			else if(sequence == sequence_end && next_animation == nullptr){
				time_counter = sequence->keytime;
				break;	
			}

			//Play the new sound, if it exists
			if(sequence->sound != nullptr){
				engine->getSoundBoard()->playSound(sequence->sound, 0);
			}
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
void Animation::draw(SDL_Renderer* renderer, uint64_t delta, float camera_x, float camera_y, double z_coord){
	// Check to see if we've been initialized
	if(this->sequence == NULL){
		return;
	}

	//Advance the animation if not paused
	if(!engine->checkState(GAME_STATE::PAUSE)){
		this->advance(delta);
	}

	Sprite* sprite = this->sequence->sprite[curr_sprite_set];
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
	draw_rect.y -= camera_y + z_coord;

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

	if(debug == true){
		//Draw hitboxes
		HitboxList* hitboxes = this->getHitboxes();
		while(hitboxes != nullptr){
			Hitbox* hitbox = hitboxes->hitbox;
			
			//Determine hitbox color depending on type
			SDL_Color color;
			memset(&color, 0, sizeof(SDL_Color));
			
			//Performing a mask, since stuff may be collision & environment
			uint64_t hitbox_type = hitbox->getType();
			if(hitbox_type & HITBOX_TYPE::COLLISION){
				color.g = 255;
			}
			else if(hitbox_type & HITBOX_TYPE::DAMAGEBOX){
				color.r = 255;
			}
			else if(hitbox_type & HITBOX_TYPE::HURTBOX){
				color.b = 255;
			}
			else if(hitbox_type & HITBOX_TYPE::ENVIRONMENT){
				color.r = 255;
				color.b = 255;
			}
			else if(hitbox_type & HITBOX_TYPE::GROUNDING_ZONE){
				color.g = 255;
				color.b = 255;
			}
			else{
				color.r = 0;
				color.g = 0;
				color.b = 0;
			}

			enum HITBOX_SHAPE shape = hitbox->getShape();
			if(shape == HITBOX_SHAPE::HIT_RECT){
				HitRect* hit_rect = (HitRect*)hitbox;
				SDL_Rect rect;
				rect.h = hit_rect->getHeight();
				rect.w = hit_rect->getWidth();
				rect.x = hit_rect->getX() - camera_x;
				rect.y = hit_rect->getY() - camera_y - hit_rect->getZOffset() - z_coord;
				unsigned int depth = hit_rect->getDepth();
				
				//Draw the base (will be white to differentiate from the rest)
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				DrawSDL_Rect(renderer, rect);
				SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

				//Draw the sides
				SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x, rect.y - depth);
				SDL_RenderDrawLine(renderer, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y - depth);
				SDL_RenderDrawLine(renderer, rect.x, rect.y + rect.h, rect.x, rect.y + rect.h - depth);
				SDL_RenderDrawLine(renderer, rect.x + rect.w, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h - depth);

				//Draw the top
				rect.y -= depth;
				DrawSDL_Rect(renderer, rect);
			}
			else if(shape == HITBOX_SHAPE::HIT_ELLIPSE){
				HitEllipse* hit_ellipse = (HitEllipse*)hitbox;
				unsigned int center_x = hit_ellipse->getX() - camera_x;
				unsigned int center_y = hit_ellipse->getY() - camera_y + hit_ellipse->getZOffset() - z_coord;
				float x_radius = hit_ellipse->getXRadius();
				float y_radius = hit_ellipse->getYRadius();
				unsigned int depth = hit_ellipse->getDepth();

				//Draw the base
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				DrawSDL_Ellipse(renderer, center_x, center_y, x_radius, y_radius);
				SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

				//Draw the sides
				unsigned int left_x = center_x - x_radius;
				unsigned int right_x = center_x + x_radius;
				SDL_RenderDrawLine(renderer, left_x, center_y, left_x, center_y - depth);
				SDL_RenderDrawLine(renderer, right_x, center_y, right_x, center_y - depth);

				//Draw the top
				center_y -= depth;
				DrawSDL_Ellipse(renderer, center_x, center_y, x_radius, y_radius);
			}

			hitboxes = hitboxes->next;
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
		for(auto& sprite_set:this->sprite_sets){
			animations->sprite[sprite_set.second]->rotation += rotate_amnt * ((direction)? -1 : 1);
		}

		HitboxList* hitboxes = animations->hitboxes;
		while(hitboxes != NULL){
			Hitbox* hitbox = hitboxes->hitbox;
			if(hitbox->getShape() == HIT_CONE){
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

/** Gets the length of a sequence
 * @return The sequence len
 */
uint16_t Animation::getSequenceLen(){
	return this->sequence_len;
}

/** Checks if the animation has a specified sprite set
 * @return true if the animation has the sprite_set, false otherwise
 */
bool Animation::hasSpriteSet(const char* sprite_set){
	return (this->sprite_sets.find(std::string(sprite_set)) != this->sprite_sets.end());
}

/** Saves the resources of the entity to a char*'s (which should be freed upon return)
 * @param file The pointer to the open file to write to
 * @param written_sprites The set of sprites that have already been written to file
 * @param written_audio The set of audio assets that have already been written to file
 * @return 0 if successful
 */
int Animation::serializeAssets(FILE* file, SerializeSet& serialize_set){
    AnimationSeq* cursor = sequence_start;
    if(cursor != NULL){
        do{
			for(auto& sprite_set:this->sprite_sets){
				if(cursor->sprite != nullptr){
					Sprite* sprite = cursor->sprite[sprite_set.second];
					//If this asset's not been saved in this file yet
					if(sprite != nullptr && serialize_set.sprite_set.find(sprite->name) == serialize_set.sprite_set.end()){
						//Use the surface from the engine; the animation's actual surface may have transparency set, masking, etc.
						SDL_Surface* surface = engine->getSurface(sprite->name);

						if(surface != nullptr){
							uint8_t resource_type = RESOURCE_TYPE::BMP;
							fwrite(&resource_type, sizeof(resource_type), 1, file);

							//Identifier
							uint16_t identifier_len = strlen(sprite->name);
							WriteVar(identifier_len, uint16_t, file);
							fwrite(sprite->name, 1, identifier_len, file);

							SerializeSurface(file, sprite->surface);

							//Log this sprite as written
							serialize_set.sprite_set.insert(sprite->name);
						}
					}
				}
			}

			Sound* sound = cursor->sound;
			//Also have a place for saving audio (once that's implemented in the system)
			if(sound != nullptr && serialize_set.audio_set.find(cursor->sound->name) == serialize_set.audio_set.end()){
				SerializeSound(file, cursor->sound);

				//Insert the audio
				serialize_set.audio_set.insert(cursor->sound->name);
			}
			cursor = cursor->next;
        } while(cursor != sequence_start && cursor != nullptr);
    }

	return 0;
}

/** Serialize animation data & write it to file
 * @param file The file to write animation data to
 * @return 0 on success
 */
int Animation::serializeData(FILE* file){
	//Store animation identifier
	uint16_t name_len = strlen(this->name);
	WriteVar(name_len, uint16_t, file);
	fwrite(this->name, 1, name_len, file);

	//Number of frames
	WriteVar(this->sequence_len, uint16_t, file);

	//Per frame
	AnimationSeq* sequence_cursor = this->sequence_start;
	if(sequence_cursor != nullptr){
		do{
			//Getting the number of sprites in this frame
			uint16_t sprite_num = 0;
			for(auto& sprite_set:sprite_sets){
				if(sequence_cursor->sprite[sprite_set.second] != nullptr){
					sprite_num++;
				}
			}

			WriteVar(sprite_num, uint16_t, file);

			//Recording the sprite & attributes in this frame
			for(auto& sprite_set:sprite_sets){
				if(sequence_cursor->sprite[sprite_set.second] != nullptr){
					const char* sprite_set_name = sprite_set.first.c_str();
					uint16_t sprite_set_name_len = strlen(sprite_set_name);
					WriteVar(sprite_set_name_len, uint16_t, file);
					fwrite(&sprite_set_name, 1, sprite_set_name_len, file);

					WriteVar(sequence_cursor->sprite[sprite_set.second]->base_x_offset, uint16_t, file);
					WriteVar(sequence_cursor->sprite[sprite_set.second]->base_y_offset, uint16_t, file);
					WriteVar(sequence_cursor->sprite[sprite_set.second]->rect->w, uint16_t, file);
					WriteVar(sequence_cursor->sprite[sprite_set.second]->rect->h, uint16_t, file);
					WriteVar((uint64_t)sequence_cursor->sprite[sprite_set.second]->rotation, uint64_t, file);
				}
			}

			//Record the keytime
			WriteVar(sequence_cursor->keytime, uint32_t, file);

			//Record the sound
			if(sequence_cursor->sound != nullptr){
				const char* sound_name = sequence_cursor->sound->name;
				uint16_t sound_name_len = strlen(sound_name);
				WriteVar(sound_name_len, uint16_t, file);
				fwrite(&sound_name, 1, sound_name_len, file);
			}
			else{
				uint16_t zero = 0;
				fwrite(&zero, sizeof(zero), 1, file);
			}

			//Record hitbox info
			HitboxList* hitbox_cursor = sequence_cursor->hitboxes;
			while(hitbox_cursor != nullptr){
				hitbox_cursor->hitbox->serializeData(file);

				hitbox_cursor = hitbox_cursor->next;
			}

			sequence_cursor = sequence_cursor->next;
		}while(sequence_cursor != nullptr && sequence_cursor != sequence_start);
	}

	return 0;
}

int Animation::deserializeData(FILE* file){
	//TODO
	return 0;
}