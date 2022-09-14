#ifndef ANIMATION_H
#define ANIMATION_H

#include "../Audio/Audio.hpp"
#include "./Hitbox/Hitbox.hpp"
#include "./Hitbox/HitEllipse/HitCone/HitCone.hpp"
#include "../Global/Global.hpp"

#include <SDL2/SDL.h>
#include <unordered_map>
#include <map>
#include <unordered_set>

struct Sprite{
	char* name;

	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect* rect;

	//X & Y offsets from the object, discluding scale
	uint32_t base_x_offset;
	uint32_t base_y_offset;

	//X & Y offsets from the object, including scale
	uint32_t curr_x_offset;
	uint32_t curr_y_offset;

	//Draw axis
	double upper_draw_axis;
	double lower_draw_axis;
};

typedef struct AnimationSequence{
	Sprite** sprite = nullptr;

	Sound* sound = nullptr;

	HitboxList* hitboxes = nullptr;
	
	unsigned int keytime = 0;

	struct AnimationSequence* next = nullptr;
} AnimationSeq;

class Animation{
	public:
		Animation(const char* name, uint16_t num_sprite_sets);
		Animation(FILE* file);
		~Animation();

		void advance(uint64_t delta);
		void start();

		Sprite* getSprite();
		Sound* getSound();
		HitboxList* getHitboxes();
		double getUpperDrawAxis();
		double getLowerDrawAxis();
		uint32_t getTimeLeft();
		bool getPaused();
		const char* getName();
		AnimationSeq* getSequenceStart();
		AnimationSeq* getSequenceEnd();
		uint16_t getSequenceLen();
		bool hasSpriteSet(const char* sprite_set);

		void setPaused(bool paused);
		int setSize(int width, int height);
		void setNextAnimation(Animation* next_animation);
		int setSpriteSet(const char* sprite_set);
		void setUpperDrawAxis(double draw_axis, int32_t sprite_set);
		void setLowerDrawAxis(double draw_axis, int32_t sprite_set);

		int addFrame(unsigned int keytime);
		int addSprite(const char* sprite_set, const char* sprite_path, double x_offset, double y_offset, int width = -1, int height = -1);
		int addSpriteSet(const char* sprite_set);
		int addHitbox(Hitbox* hitbox);
		int addHitbox(Hitbox* hitbox, int sequence_num);
		int addSound(const char* sound_id, int sequence_num);

		void draw(SDL_Renderer* renderer, uint64_t delta, int x_off, int y_off, int width, int height);

		int serializeAssets(FILE* file, SerializeSet& serialize_set);
		int serializeData(FILE* file);

		int deserializeData(FILE* file);

	private:
		//Name
		char* name;

		//The image sequence and start of image sequence
		AnimationSeq* sequence;
		AnimationSeq* sequence_end;
		AnimationSeq* sequence_start;

		//The next animation to be played (if null the animation will stop at the end, if it's a reference to itself it'll loop).
		//Uses the animation rather than AnimationSeq so that we can enact lazy loading while bringing files in
		Animation* next_animation;

		//Keeps track of the current sprite set, and the number of sprite sets for this animation
		uint16_t curr_sprite_set;
		std::map<std::string, uint16_t> sprite_sets;
		uint16_t num_sprite_sets;
		uint16_t sprite_set_counter;

		//The number of AnimationSeqs in the Animation
		uint16_t sequence_len;

		//Pointers to the X and Y base coords
		double* x_base;
		double* y_base;

		//The time counter
		unsigned int time_counter;

		//If the animation is paused
		bool paused;

		bool isAnimated();
};

struct AnimationList{
	Animation* animation;
	struct AnimationList* next;
};

#endif
