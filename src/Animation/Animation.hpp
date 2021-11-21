#ifndef ANIMATION_H
#define ANIMATION_H

#include "../HashTable/TextureHash/TextureHash.hpp"
#include "./Hitbox/Hitbox.hpp"
#include "./Hitbox/HitEllipse/HitCone/HitCone.hpp"

#include <SFML/Graphics.hpp>
#include <SDL2/SDL.h>
#include <unordered_map>

extern TextureHash* texture_hash;

struct Sprite{
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
		Animation(float* x_base, float* y_base);
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

		void setPaused(bool paused);
		void setSize(int width, int height);
		void setScale(float x_scale, float y_scale);

		void addFrame(const char* sprite_path, unsigned int keytime, float x_offset, float y_offset, int width = -1, int height = -1);
		void addHitbox(Hitbox* hitbox);
		void addHitbox(Hitbox* hitbox, int hitbox_num);

		void draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y);

		void rotate(int direction, float rotation_amnt);


	private:

		//The image sequence and start of image sequence
		AnimationSeq* sequence;
		AnimationSeq* sequence_end;
		AnimationSeq* sequence_start;

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