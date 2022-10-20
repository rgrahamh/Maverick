#include "./Animation.hpp"

#include "Maverick/Engine/Engine.hpp"
#include "Maverick/Global/Global.hpp"
#include "Maverick/FileHandler/FileHandler.hpp"

extern bool debug;

//FINISH MULTIPLE SPRITES

Animation::Animation(const std::string& name, uint16_t num_sprite_sets){
    this->name = name;
	this->time_counter = 0;
	this->paused = false;
	this->looped = false;
	this->scale = 1.0;

	this->next_animation = nullptr;
}

Animation::Animation(FILE* file){
	this->deserializeData(file);
}

Animation::~Animation(){
	for(auto& frame : sequence){
		for(auto& sprite_set : frame->sprites){
			for(auto& sprite : sprite_set.second){
				delete sprite;
			}
		}
		for(auto& hitboxes : frame->hitboxes){
			delete hitboxes;
		}

		delete frame->sound;
	}
}

const std::list<Sprite*>* Animation::getSprites() const{
	if(this->sequence.size() < keyframe && this->sequence[keyframe]->sprites.find(curr_sprite_set) != this->sequence[keyframe]->sprites.end()){
		return &sequence[keyframe]->sprites[curr_sprite_set];
	}
	else{
		return nullptr;
	}
}

const Sound* Animation::getSound() const{
	if(likely(sequence.size() > keyframe)){
		return sequence[keyframe]->sound;
	}
	else{
		return nullptr;
	}
}

const std::list<Hitbox*>* Animation::getHitboxes() const{
	if(likely(sequence.size() > keyframe)){
		return &sequence[keyframe]->hitboxes;
	}
	else{
		return nullptr;
	}
}

double Animation::getUpperDrawAxis(double y_base){
	if(likely(sequence.size() > keyframe)){
		//If draw_axis is set
		Frame* frame = sequence[keyframe];
		if(likely(frame->upper_draw_axis != -1)){
			return y_base + frame->upper_draw_axis;
		}
	}
	//If the draw axis isn't set
	return y_base;
}

double Animation::getLowerDrawAxis(double y_base){
	if(likely(sequence.size() > keyframe)){
		//If draw_axis is set
		Frame* frame = sequence[keyframe];
		if(likely(frame->lower_draw_axis != -1)){
			return y_base + frame->lower_draw_axis;
		}
	}
	//If the draw axis isn't set
	return y_base;
}

uint32_t Animation::getTimeLeft(){
	if(!this->isAnimated()){
		return 0;
	}

	int32_t time_left = 0;
	for(int i = keyframe; i < sequence.size(); i++){
		time_left += sequence[i]->keytime;
	}

	if(time_left > this->time_counter){
		return time_left - this->time_counter;
	}
	else{
		return 0;
	}
}

bool Animation::getPaused(){
	return this->paused;
}

unsigned int Animation::getWidth(){
	if(unlikely(sequence.size() <= keyframe || sequence[keyframe]->sprites.find(curr_sprite_set) == sequence[keyframe]->sprites.end() || sequence[keyframe]->sprites[curr_sprite_set].empty() || sequence[keyframe]->sprites[curr_sprite_set].front()->surface == nullptr)){
		return 0;
	}

	return sequence[keyframe]->sprites[curr_sprite_set].front()->surface->w * this->scale;
}

unsigned int Animation::getHeight(){
	if(unlikely(sequence.size() <= keyframe || sequence[keyframe]->sprites.find(curr_sprite_set) == sequence[keyframe]->sprites.end() || sequence[keyframe]->sprites[curr_sprite_set].empty() || sequence[keyframe]->sprites[curr_sprite_set].front()->surface == nullptr)){
		return 0;
	}

	return sequence[keyframe]->sprites[curr_sprite_set].front()->surface->h * this->scale;
}

void Animation::setPaused(bool paused){
	this->paused = paused;
}

/** If the animation is animated
 * @return If the animation is animated (if it has multiple frames, and hasn't hit the end of the animation)
 */
bool Animation::isAnimated(){
	return sequence.size() <= 1; 
}

int Animation::addFrame(unsigned int keytime){
	Frame* frame = new Frame();
	frame->keytime = keytime;
	frame->sound = nullptr;
	frame->lower_draw_axis = -1.0;
	frame->upper_draw_axis = -1.0;
	sequence.push_back(frame);

	return 0;
}

int Animation::addSprite(unsigned int frame_num, const char* sprite_set, const char* sprite_path, double x_offset, double y_offset, int width, int height){
	if(sequence.size() <= frame_num || sequence[frame_num]->sprites.find(sprite_set) == sequence[frame_num]->sprites.end()){
		if(this->addSpriteSet(sprite_set) != 0){
			return -1;
		}
	}

	//Getting the texture
	SDL_Surface* surface = Engine::getInstance()->getSurface(sprite_path);
	if(surface == nullptr){
		printf("Cannot find image %s!\n", sprite_path);
		return -1;
	}

	//Create the sprite
	Sprite* sprite = new Sprite();
	sprite->name = StrDeepCopy(sprite_path);

	sprite->x_offset = x_offset;
	sprite->y_offset = y_offset;
	sprite->surface = surface;
	sprite->texture = nullptr;

	sequence[frame_num]->sprites[sprite_set].push_back(sprite);

	return 0;
}

int Animation::addHitbox(Hitbox* hitbox){
	int retVal = -1;
	for(auto& frame : sequence){
		frame->hitboxes.push_back(hitbox);
		retVal = 0;
	}

	return retVal;
}

int Animation::addHitbox(Hitbox* hitbox, int sequence_num){
	if(sequence.size() < sequence_num || hitbox == nullptr){
		return -1;
	}

	sequence[sequence_num]->hitboxes.push_back(hitbox);
	return 0;
}

int Animation::addSound(const std::string& sound_id, int sequence_num){
	Engine* engine = Engine::getInstance();
	Sound* sound = engine->getSound(sound_id);
	if(sequence.size() < sequence_num || sound == nullptr){
		return -1;
	}

	sequence[sequence_num]->sound = sound;
	return 0;
}

void Animation::setNextAnimation(Animation* next_animation){
	this->next_animation = next_animation;
}

int Animation::setSpriteSet(const std::string& new_sprite_set){
	for(auto& frame : sequence){
		for(auto& sprite_set : frame->sprites){
			if(sprite_set.first == new_sprite_set){
				this->curr_sprite_set = new_sprite_set;
				return 0;
			}
		}
	}
	return -1;
}

void Animation::setUpperDrawAxis(double upper_draw_axis, int32_t frame_num){
	if(sequence.size() <= frame_num){
		return;
	}

	sequence[frame_num]->upper_draw_axis = upper_draw_axis;
}

void Animation::setLowerDrawAxis(double lower_draw_axis, int32_t frame_num){
	if(sequence.size() <= frame_num){
		return;
	}

	sequence[frame_num]->lower_draw_axis = lower_draw_axis;
}

void Animation::setScale(double scale){
	this->scale = scale;
}

void Animation::advance(uint64_t delta){
	if(this->isAnimated() && !this->paused){
		time_counter += delta;
		//If we've exceeded the keytime for this frame
		if(sequence[keyframe]->keytime <= time_counter){
			time_counter -= sequence[keyframe]->keytime;

			//The case where we shouldn't loop
			if(keyframe == sequence.size() - 1 && !looped){
				time_counter = 0;
			}
			//The looping case
			else{
				keyframe++;
				keyframe %= sequence.size();

				//Play the new sound, if it exists
				if(sequence[keyframe]->sound != nullptr){
					Engine::getInstance()->getSoundBoard()->playSound(sequence[keyframe]->sound, 0);
				}
			}
		}
	}
}

void Animation::start(){
	keyframe = 0;
	time_counter = 0;
}

void Animation::draw(uint64_t delta, int x_off, int y_off){
	// Check to see if we've been initialized
	if(this->sequence.size() < keyframe){
		return;
	}

	//Advance the animation if not paused
	if(!Engine::getInstance()->checkState(GAME_STATE::PAUSE)){
		this->advance(delta);
	}

	std::list<Sprite*>& sprites = this->sequence[keyframe]->sprites[curr_sprite_set];
	for(auto& sprite : sprites)
	{
		if(sprite == nullptr || sprite->surface == nullptr){
			return;
		}

		//Update the sprite position
		//Should there be a native X & a native Y scale?
		Engine* engine = Engine::getInstance();
		int width, height;
		SDL_GetRendererOutputSize(engine->getRenderer(), &width, &height);
		double native_scale = engine->getNativeScale();
		SDL_Rect curr_rect;
		curr_rect.x = (x_off + sprite->x_offset) * ((double)width / (double)DESIGN_SCREEN_WIDTH);
		curr_rect.y = (y_off + sprite->y_offset) * ((double)height / (double)DESIGN_SCREEN_HEIGHT);

		double obj_scale = native_scale * this->scale;
		curr_rect.w = sprite->surface->w * obj_scale;
		curr_rect.h = sprite->surface->h * obj_scale;

		SDL_Renderer* renderer = engine->getRenderer();
		if(sprite->texture == NULL){
			sprite->texture = SDL_CreateTextureFromSurface(renderer, sprite->surface);
		}

		if(sprite->texture == nullptr){
			return;
		}

		//Draw the sprite
		if(SDL_RenderCopy(renderer, sprite->texture, NULL, &curr_rect)){
			printf("%s\n", SDL_GetError());
		}

		//Draw hitboxes in debug mode
		if(debug == true){
			const std::list<Hitbox*>* hitboxes = this->getHitboxes();
			if(hitboxes == nullptr){
				return;
			}

			for(auto& hitbox : *hitboxes){
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
					rect.h = hit_rect->getHeight() * native_scale;
					rect.w = hit_rect->getWidth() * native_scale;
					rect.x = (hit_rect->getX() - x_off) * native_scale;
					rect.y = (hit_rect->getY() - y_off) * native_scale;
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
			}
		}
	}
}

void Animation::draw(uint64_t delta, const SDL_Rect& draw_area){
	// Check to see if we've been initialized
	if(this->sequence.size() < keyframe){
		return;
	}

	//Advance the animation if not paused
	Engine* engine = Engine::getInstance();
	if(!engine->checkState(GAME_STATE::PAUSE)){
		this->advance(delta);
	}

	std::list<Sprite*>& sprites = this->sequence[keyframe]->sprites[curr_sprite_set];
	for(auto& sprite : sprites)
	{
		if(sprite == nullptr || sprite->surface == NULL){
			continue;
		}

		SDL_Renderer* renderer = engine->getRenderer();
		if(sprite->texture == NULL){
			sprite->texture = SDL_CreateTextureFromSurface(renderer, sprite->surface);
		}

		SDL_Rect draw_rect;
		draw_rect.x = draw_area.x + sprite->x_offset;
		draw_rect.y = draw_area.y + sprite->y_offset;

		//If draw area is given, follow it exactly (not scaled). Otherwise, guess from the surface size * scale.
		if(draw_area.w == -1){
			draw_rect.w = sprite->surface->w * this->scale;
		}
		else{
			draw_rect.w = draw_area.w;
		}

		if(draw_area.h == -1){
			draw_rect.h = sprite->surface->h * this->scale;
		}
		else{
			draw_rect.h = draw_area.h;
		}

		//Draw the sprite
		if(SDL_RenderCopy(renderer, sprite->texture, NULL, &draw_rect)){
			printf("%s\n", SDL_GetError());
		}
	}
}

const std::string& Animation::getName(){
	return this->name;
}

bool Animation::hasSpriteSet(const char* sprite_set){
	if(sequence.size() < keyframe){
		return false;
	}
	return sequence[keyframe]->sprites.find(std::string(sprite_set)) != sequence[keyframe]->sprites.end();
}

int Animation::serializeAssets(FILE* file, SerializeSet& serialize_set){
	for(auto& frame : sequence){
		for(auto& sprite_set : frame->sprites){
			//Per sprite
			for(auto& sprite : sprite_set.second){
				//Write the sprite name

				uint8_t asset_type = RESOURCE_TYPE::BMP;
				WriteVar(asset_type, file);

				uint16_t identifier_len = sprite->name.size();
				WriteVar(identifier_len, file);

				fwrite(sprite->name.data(), 1, sprite->name.size(), file);

				SerializeSurface(file, sprite->surface);
			}
		}

		SerializeSound(file, frame->sound);
    }

	return 0;
}

int Animation::serializeData(FILE* file){
	WriteStr(name, file);

	//Number of frames
	WriteVar(sequence.size(), file);

	for(auto& frame : sequence){
		WriteVar(frame->keytime, file);
		WriteVar(frame->upper_draw_axis, file);
		WriteVar(frame->lower_draw_axis, file);
		for(auto& sprite_set : frame->sprites){
			//Write the sprite set name
			WriteStr(sprite_set.first, file);

			//Per sprite
			for(auto& sprite : sprite_set.second){
				//Write the sprite name
				WriteStr(sprite->name, file);
				WriteVar(sprite->x_offset, file);
				WriteVar(sprite->y_offset, file);
			}
		}

		uint16_t hitbox_num = frame->hitboxes.size();
		WriteVar(hitbox_num, file);
		for(auto& hitbox : frame->hitboxes){
			hitbox->serializeData(file);
		}

	}

	WriteVar(scale, file);
	WriteVar(looped, file);
	WriteStr(curr_sprite_set, file);

	return 0;
}

int Animation::deserializeData(FILE* file){
	if(file == nullptr){
		return -1;
	}

	//Name length
	ReadStr(name, file);

	uint16_t num_frames;
	ReadVar(num_frames, file);

	Engine* engine = Engine::getInstance();
	for(uint16_t frame_num = 0; frame_num < num_frames; frame_num++){
		Frame* frame = new Frame;
		ReadVar(frame->keytime, file);
		ReadVar(frame->upper_draw_axis, file);
		ReadVar(frame->lower_draw_axis, file);

		uint16_t num_sprites;
		ReadVar(num_sprites, file);
		for(unsigned int sprite_num = 0; sprite_num < num_sprites; sprite_num++){
			Sprite* sprite = new Sprite;
			ReadStr(sprite->name, file);
			sprite->surface = engine->getSurface(sprite->name);

			ReadVar(sprite->x_offset, file);
			ReadVar(sprite->y_offset, file);
		}

		uint16_t num_hitboxes;
		ReadVar(num_hitboxes, file);

		sequence.push_back(frame);
	}

	ReadVar(scale, file);
	ReadVar(looped, file);
	ReadStr(curr_sprite_set, file);

	return 0;
}