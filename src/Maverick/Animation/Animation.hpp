#ifndef ANIMATION_H
#define ANIMATION_H

#include "../HashTable/TextureHash/TextureHash.hpp"
#include "./Hitbox/Hitbox.hpp"
#include "./Hitbox/HitEllipse/HitCone/HitCone.hpp"
#include "../Utility/Utility.hpp"

#include <SFML/Graphics.hpp>
#include <SDL2/SDL.h>
#include <unordered_map>
#include <unordered_set>

extern TextureHash* texture_hash;

struct Sprite{
	char* name;

	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect* rect;

	//X & Y offsets from the object, discluding scale
	float base_x_offset;
	float base_y_offset;

	//X & Y offsets from the object, including scale
	float curr_x_offset;
	float curr_y_offset;

	//Rotation (clockwise, in degrees)
	double rotation;
};

typedef struct AnimationSequence{
	Sprite* sprite;

	HitboxList* hitboxes;
	
	unsigned int keytime;

	struct AnimationSequence* next;
} AnimationSeq;

class Animation{
	public:
		Animation(const char* name, float* x_base, float* y_base);
		void freeFrame(AnimationSeq* );
		~Animation();

		void advance(uint32_t delta);
		void start();

		Sprite* getSprite();
		HitboxList* getHitboxes();
		unsigned char getDrawLayer();
		float getDrawAxis();
		uint32_t getTimeLeft();
		bool getPaused();
		const char* getName();
		AnimationSeq* getSequenceStart();
		AnimationSeq* getSequenceEnd();

		void setPaused(bool paused);
		int setSize(int width, int height);
		int setScale(float x_scale, float y_scale);
		void setNextAnimation(Animation* next_animation);

		int addFrame(const char* sprite_path, unsigned int keytime, float x_offset, float y_offset, int width = -1, int height = -1);
		int addHitbox(Hitbox* hitbox);
		int addHitbox(Hitbox* hitbox, int hitbox_num);

		void draw(SDL_Renderer* renderer, uint32_t delta, float camera_x, float camera_y);

		void rotate(int direction, float rotation_amnt);

		void saveResources(FILE* file, std::unordered_set<std::string>& sprite_set, std::unordered_set<std::string>& audio_set);

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

		//Pointers to the X and Y base coords
		float* x_base;
		float* y_base;

		//Scale of the height and width
		float x_scale;
		float y_scale;

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