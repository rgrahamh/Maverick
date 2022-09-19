#include "./Animation.hpp"

#include "Maverick/Engine/Engine.hpp"
#include "Maverick/Global/Global.hpp"

extern bool debug;

//FINISH MULTIPLE SPRITES

Animation::Animation(const char* name, uint16_t num_sprite_sets){
    this->name = StrDeepCopy(name);
	this->sequence = NULL;
	this->sequence_start = NULL;
	this->sequence_end = NULL;
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

Sprite* Animation::getSprite(){
	if(this->sequence != nullptr){
		return this->sequence->sprite[curr_sprite_set];
	}
	else{
		return nullptr;
	}
}

Sound* Animation::getSound(){
	if(this->sequence != nullptr){
		return this->sequence->sound;
	}
	else{
		return nullptr;
	}
}

HitboxList* Animation::getHitboxes(){
	if(this->sequence){
		return this->sequence->hitboxes;
	}
	else{
		return nullptr;
	}
}

double Animation::getUpperDrawAxis(){
	if(likely(this->sequence != NULL && this->sequence->sprite[curr_sprite_set] != NULL)){
		//If draw_axis is set
		Sprite* sprite = this->sequence->sprite[curr_sprite_set];
		if(sprite->upper_draw_axis != -1){
			return *this->y_base + sprite->upper_draw_axis + sprite->y_offset;
		}
		//Otherwise, interpolate from the sequence
		else{
			return *this->y_base + sprite->y_offset;
		}
	}
	//If we can't find the draw axis any other way
	else{
		return 0.0;
	}
}

double Animation::getLowerDrawAxis(){
	if(likely(this->sequence != NULL && this->sequence->sprite[curr_sprite_set] != NULL)){
		//If draw_axis is set
		Sprite* sprite = this->sequence->sprite[curr_sprite_set];
		if(sprite->lower_draw_axis != -1){
			return *this->y_base + sprite->lower_draw_axis + sprite->y_offset;
		}
		//Otherwise, interpolate from the sequence
		else{
			return *this->y_base + sprite->surface->h + sprite->y_offset;
		}
	}
	//If we can't find the draw axis any other way
	else{
		return 0.0;
	}
}

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

bool Animation::getPaused(){
	return this->paused;
}

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

	//Getting the texture
	SDL_Surface* surface;
	if((surface = Engine::getInstance()->getSurface(sprite_path)) == NULL){
		if(surface == nullptr){
			printf("Cannot find image %s!\n", sprite_path);
			return -1;
		}
	}

	//Create the sprite
	Sprite* sprite = new Sprite();
	sprite->name = StrDeepCopy(sprite_path);
	sprite->lower_draw_axis = -1.0;
	sprite->upper_draw_axis = -1.0;

	sprite->x_offset = x_offset;
	sprite->y_offset = y_offset;
	sprite->surface = surface;
	sprite->texture = nullptr;

	sequence_cursor->sprite[sprite_set_idx] = sprite;

	return 0;
}

int Animation::addSpriteSet(const char* sprite_set){
	if(sprite_set_counter < num_sprite_sets && this->sprite_sets.find(std::string(sprite_set)) == this->sprite_sets.end()){
		this->sprite_sets[std::string(sprite_set)] = sprite_set_counter++;
		return 0;
	}
	return -1;
}

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

int Animation::addHitbox(Hitbox* hitbox, int sequence_num){
	if(this->sequence_start != nullptr){
		if(sequence_num == -1){
			HitboxList* new_hitbox = new HitboxList;
			new_hitbox->hitbox = hitbox;
			new_hitbox->next = NULL;
			if(this->sequence_end->hitboxes == NULL){
				this->sequence_end->hitboxes = new_hitbox;
			}
			else{
				this->sequence_end->hitboxes->next = new_hitbox;
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

int Animation::addSound(const char* sound_id, int sequence_num){
	if(sound_id == nullptr){
		return -1;
	}

	Sound* sound = Engine::getInstance()->getSound(sound_id);
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

void Animation::setNextAnimation(Animation* next_animation){
	this->next_animation = next_animation;
}

int Animation::setSpriteSet(const char* sprite_set){
	if(this->sprite_sets.find(std::string(sprite_set)) != this->sprite_sets.end()){
		this->curr_sprite_set = sprite_sets[std::string(sprite_set)];
		return 0;
	}
	return -1;
}

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
				Engine::getInstance()->getSoundBoard()->playSound(sequence->sound, 0);
			}
		}
	}
}

void Animation::start(){
	sequence = sequence_start;
	time_counter = 0;
}

void Animation::draw(SDL_Renderer* renderer, uint64_t delta, int x_off, int y_off){
	// Check to see if we've been initialized
	if(this->sequence == NULL){
		return;
	}

	//Advance the animation if not paused
	if(!Engine::getInstance()->checkState(GAME_STATE::PAUSE)){
		this->advance(delta);
	}

	Sprite* sprite = this->sequence->sprite[curr_sprite_set];
	if(sprite == nullptr){
		return;
	}

	//Update the sprite position
	SDL_Rect curr_rect;
	curr_rect.x = x_off + sprite->x_offset;
	curr_rect.y = y_off + sprite->y_offset;
	curr_rect.w = sprite->surface->w;
	curr_rect.h = sprite->surface->h;

	if(sprite->texture == NULL && sprite->surface != NULL){
		sprite->texture = SDL_CreateTextureFromSurface(renderer, sprite->surface);
	}

	//Draw the sprite
	if(SDL_RenderCopy(renderer, sprite->texture, NULL, &curr_rect)){
		printf("%s\n", SDL_GetError());
	}

	//Draw hitboxes in debug mode
	if(debug == true){
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
				rect.x = hit_rect->getX() - x_off;
				rect.y = hit_rect->getY() - y_off;
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
				unsigned int center_x = hit_ellipse->getX() - x_off;
				unsigned int center_y = hit_ellipse->getY() - y_off + hit_ellipse->getZOffset();
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


void Animation::draw(SDL_Renderer* renderer, uint64_t delta, SDL_Rect& draw_area){
	// Check to see if we've been initialized
	if(this->sequence == NULL){
		return;
	}

	//Advance the animation if not paused
	if(!Engine::getInstance()->checkState(GAME_STATE::PAUSE)){
		this->advance(delta);
	}

	Sprite* sprite = this->sequence->sprite[curr_sprite_set];
	if(sprite == nullptr){
		return;
	}

	if(sprite->texture == NULL && sprite->surface != NULL){
		sprite->texture = SDL_CreateTextureFromSurface(renderer, sprite->surface);
	}

	SDL_Rect draw_rect;
	draw_rect.x = draw_area.x + sprite->x_offset;
	draw_rect.y = draw_area.y + sprite->y_offset;

	if(draw_area.w == -1){
		draw_rect.w = sprite->surface->w;
	}
	else{
		draw_rect.w = draw_area.w;
	}

	if(draw_area.h == -1){
		draw_rect.h = sprite->surface->h;
	}
	else{
		draw_rect.h = draw_area.h;
	}

	//Draw the sprite
	if(SDL_RenderCopy(renderer, sprite->texture, NULL, &draw_rect)){
		printf("%s\n", SDL_GetError());
	}
}

AnimationSeq* Animation::getSequenceStart(){
	return this->sequence_start;
}

AnimationSeq* Animation::getSequenceEnd(){
	return this->sequence_end;
}

const char* Animation::getName(){
	return this->name;
}

uint16_t Animation::getSequenceLen(){
	return this->sequence_len;
}

bool Animation::hasSpriteSet(const char* sprite_set){
	return (this->sprite_sets.find(std::string(sprite_set)) != this->sprite_sets.end());
}

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
						SDL_Surface* surface = Engine::getInstance()->getSurface(sprite->name);

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

					WriteVar(sequence_cursor->sprite[sprite_set.second]->x_offset, uint16_t, file);
					WriteVar(sequence_cursor->sprite[sprite_set.second]->y_offset, uint16_t, file);
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