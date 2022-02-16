#include "./Animation.hpp"

#include "../Engine/Engine.hpp"

extern Engine* engine;


//FINISH MULTIPLE SPRITES

/** The parameterized constructor of the Animation
 * @param x_base A pointer to the int of the base object's X location
 * @param y_base A pointer to the int of the base object's Y location
 */
Animation::Animation(const char* name, double* x_base, double* y_base, uint16_t num_sprite_sets){
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
	this->num_sprite_sets = num_sprite_sets;
	this->sprite_set_counter = 0;
	this->curr_sprite_set = 0;
	this->draw_axis = -1.0;

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

			for(auto& sprite_set:this->sprite_sets){
				if(cursor->sprite != nullptr){
					if(cursor->sprite[sprite_set.second]->rect != nullptr){
						free(cursor->sprite[sprite_set.second]->rect);
					}

					if(cursor->sprite[sprite_set.second]->texture != nullptr){
						SDL_DestroyTexture(cursor->sprite[sprite_set.second]->texture);
					}
				}

				delete cursor->sprite[sprite_set.second];
			}

			delete cursor->sprite;

			AnimationSeq* tmp;
			tmp = cursor;
			cursor = cursor->next;
			free(tmp);
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
	SDL_Rect* curr_rect = this->sequence->sprite[curr_sprite_set]->rect;
	curr_rect->x = *this->x_base;
	curr_rect->y = *this->y_base;

	//If draw_axis is set
	if(this->draw_axis != -1){
		return this->draw_axis;
	}
	//Otherwise, interpolate from the sequence
	else if(this->sequence != NULL){
		return curr_rect->y + curr_rect->h + this->sequence->sprite[curr_sprite_set]->curr_y_offset;
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
 * @param sprite_path The filepath of the sprite
 * @param x_offset The X offset of the new sprite
 * @param y_offset The Y offset of the new sprite
 * @param width The width of the new sprite 
 * @param height The height of the new sprite
 */
int Animation::addSprite(const char* sprite_set, const char* sprite_path, double x_offset, double y_offset, int width, int height){
	if(this->sprite_sets.find(sprite_set) == this->sprite_sets.end()){
		return -1;
	}
	uint16_t sprite_set_idx = this->sprite_sets[sprite_set];

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

	unsigned int sprite_name_len = strlen(sprite_path);
	char* sprite_name = (char*)malloc(sprite_name_len + 1);
	memcpy(sprite_name, sprite_path, sprite_name_len);
	sprite_name[sprite_name_len] = '\0';

	//Create the sprite
	Sprite* sprite = new Sprite();
	sprite->name = sprite_name;
	sprite->rotation = 0.0;

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
 * @param sprite_set The sprite set you'd like to add
 */
int Animation::addSpriteSet(const char* sprite_set){
	if(sprite_set_counter < num_sprite_sets && this->sprite_sets.find(sprite_set) == this->sprite_sets.end()){
		this->sprite_sets[sprite_set] = sprite_set_counter++;
		return 0;
	}
	return -1;
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

/** Sets the size of the animation
 * @param x_scale The X size
 * @param y_scale the Y size
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

int Animation::setSpriteSet(const char* sprite_set){
	if(this->sprite_sets.find(sprite_set) != this->sprite_sets.end()){
		this->curr_sprite_set = sprite_sets[sprite_set];
		return 0;
	}
	return -1;
}

/** The draw axis offset, in pixels (from the top left of the animation).
 * @param offset The draw axis offset, from the top left of the animation. -1 assumes position based off of the img size
 */
void Animation::setDrawAxis(double draw_axis){
	this->draw_axis = draw_axis;
}

/** Advances the animation by a frame
 */
void Animation::advance(uint64_t delta){
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
	return (this->sprite_sets.find(sprite_set) != this->sprite_sets.end());
}

/** Saves the resources of the entity to a char*'s (which should be freed upon return)
 * @param file The pointer to the open file to write to
 * @param written_sprites The set of sprites that have already been written to file
 * @param written_audio The set of audio assets that have already been written to file
 * @param written_music The set of music assets that have already been written to file (used just by objects that handle music)
 * @return The number of bytes that *buff_ptr is
 */
int Animation::serializeAssets(FILE* file, std::unordered_set<std::string>& written_sprites, std::unordered_set<std::string>& written_audio){
    AnimationSeq* cursor = sequence_start;
    if(cursor != NULL){
        do{
			for(auto& sprite_set:this->sprite_sets){
				if(cursor->sprite != nullptr){
					Sprite* sprite = cursor->sprite[sprite_set.second];
					//If this asset's not been saved in this file yet
					if(sprite != nullptr && written_sprites.find(sprite->name) == written_sprites.end()){
						//Use the surface from the engine; the animation's actual surface may have transparency set, masking, etc.
						SDL_Surface* surface = engine->getSurface(sprite->name);

						if(surface != nullptr){
							//Gather necessary info
							//Width/Height are naturally ints (so size varies), meaning we need to truncate first
							uint32_t width = surface->w;
							uint32_t width_swap = EndianSwap(&width);
							uint32_t height = surface->h;
							uint32_t height_swap = EndianSwap(&height);

							//Bit depth of the image
							uint8_t depth = surface->format->BitsPerPixel;

							//The RGBA masks
							uint32_t rmask = EndianSwap(&surface->format->Rmask);
							uint32_t gmask = EndianSwap(&surface->format->Gmask);
							uint32_t bmask = EndianSwap(&surface->format->Bmask);
							uint32_t amask = EndianSwap(&surface->format->Amask);

							//Identifier len
							uint16_t identifier_len = strlen(sprite->name);
							uint16_t identifier_len_swapped = EndianSwap(&identifier_len);

							uint8_t asset_type = RESOURCE_TYPE::BMP;
							fwrite(&asset_type, 1, 1, file);

							//Identifier
							fwrite(&identifier_len_swapped, 2, 1, file);
							fwrite(sprite->name, 1, identifier_len, file);

							//Write the image header info
							fwrite(&width_swap, sizeof(width_swap), 1, file);
							fwrite(&height_swap, sizeof(height_swap), 1, file);
							fwrite(&depth, 1, 1, file);
							fwrite(&rmask, sizeof(rmask), 1, file);
							fwrite(&gmask, sizeof(gmask), 1, file);
							fwrite(&bmask, sizeof(bmask), 1, file);
							fwrite(&amask, sizeof(amask), 1, file);

							//Write the actual image data ((w * h * bpp) bytes)
							fwrite(&surface->format->BytesPerPixel, 1, sizeof(surface->format->BytesPerPixel), file);
							fwrite(surface->pixels, 1, width * height * surface->format->BytesPerPixel, file);

							//Log this sprite as written
							written_sprites.insert(sprite->name);
						}
					}
				}
			}

			Sound* sound = cursor->sound;
			//Also have a place for saving audio (once that's implemented in the system)
			if(sound != nullptr && written_audio.find(cursor->sound->name) == written_audio.end()){
				//Identifier len
				uint16_t identifier_len = strlen(cursor->sound->name);
				uint16_t identifier_len_swapped = EndianSwap(&identifier_len);

				uint8_t asset_type = RESOURCE_TYPE::SOUND;
				fwrite(&asset_type, 1, 1, file);

				//Identifier
				fwrite(&identifier_len_swapped, 2, 1, file);
				fwrite(cursor->sound->name, 1, identifier_len, file);

				//Writing the audio buffer (from the mixer chunk) to file
				uint32_t audio_len = EndianSwap(&cursor->sound->sample->alen);
				fwrite(&audio_len, 1, 1, file);
				fwrite(cursor->sound->sample->abuf, 1, cursor->sound->sample->alen, file);

				//Insert the audio
				written_audio.insert(cursor->sound->name);
			}
			cursor = cursor->next;
        } while(cursor != sequence_start && cursor != nullptr);
    }

	return 0;
}

int Animation::serializeData(FILE* file){
	//Store animation identifier
	uint16_t name_len = strlen(this->name);
	uint16_t name_len_swap = EndianSwap(&name_len);
	fwrite(&name_len_swap, sizeof(name_len_swap), 1, file);
	fwrite(this->name, 1, name_len, file);

	//Number of frames
	uint16_t sequence_len_swap = EndianSwap(&this->sequence_len);
	fwrite(&sequence_len_swap, sizeof(sequence_len_swap), 1, file);

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

			uint16_t sprite_num_swap = EndianSwap(&sprite_num);
			fwrite(&sprite_num_swap, sizeof(sprite_num_swap), 1, file);

			//Recording the sprite & attributes in this frame
			for(auto& sprite_set:sprite_sets){
				if(sequence_cursor->sprite[sprite_set.second] != nullptr){
					const char* sprite_set_name = sprite_set.first.c_str();
					uint16_t sprite_set_name_len = strlen(sprite_set_name);
					uint16_t sprite_set_name_len_swap = EndianSwap(&sprite_set_name_len);
					fwrite(&sprite_set_name_len_swap, sizeof(sprite_set_name_len_swap), 1, file);
					fwrite(&sprite_set_name, 1, sprite_set_name_len, file);

					uint16_t x_offset_swap = EndianSwap(&sequence_cursor->sprite[sprite_set.second]->base_x_offset);
					fwrite(&x_offset_swap, sizeof(x_offset_swap), 1, file);

					uint16_t y_offset_swap = EndianSwap(&sequence_cursor->sprite[sprite_set.second]->base_y_offset);
					fwrite(&y_offset_swap, sizeof(y_offset_swap), 1, file);

					uint16_t width_swap = EndianSwap(&sequence_cursor->sprite[sprite_set.second]->rect->w);
					fwrite(&width_swap, sizeof(width_swap), 1, file);

					uint16_t height_swap = EndianSwap(&sequence_cursor->sprite[sprite_set.second]->rect->h);
					fwrite(&height_swap, sizeof(height_swap), 1, file);

					uint64_t rotation_swap = EndianSwap((uint64_t*)&sequence_cursor->sprite[sprite_set.second]->rotation);
					fwrite(&rotation_swap, sizeof(rotation_swap), 1, file);
				}
			}

			//Record the keytime
			uint32_t keytime_swap = EndianSwap(&sequence_cursor->keytime);
			fwrite(&keytime_swap, sizeof(keytime_swap), 1, file);

			//Record the sound
			if(sequence_cursor->sound != nullptr){
				const char* sound_name = sequence_cursor->sound->name;
				uint16_t sound_name_len = strlen(sound_name);
				uint16_t sound_name_swapped = EndianSwap(&sound_name_len);
				fwrite(&sound_name_swapped, sizeof(sound_name_swapped), 1, file);
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