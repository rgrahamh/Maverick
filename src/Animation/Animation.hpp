#ifndef ANIMATION_H
#define ANIMATION_H

#include "./SpriteHash/SpriteHash.hpp"

#include <SFML/Graphics.hpp>
#include <unordered_map>

//The sequence of images in the animation; should be a circularly linked list.

//MAKE SURE TO IMPLEMENT THIS AS CIRCULARLY LINKED
typedef struct SpriteList{
	sf::Sprite* sprite;
	float x_offset;
	float y_offset;
	unsigned int keyframe;
	struct SpriteList* next;
} SpriteLst;

extern SpriteHash* sprite_hash;

class Animation{
	public:
		Animation(float* x_base, float* y_base, bool animated);
		~Animation();

		void advance();
		void start();
		void addFrame(const char* sprite_path, unsigned int keyframe, float x_offset, float y_offset);
		void setScale(float x_scale, float y_scale);
		sf::Sprite* getSprite();

	private:
		//The image sequence and start of image sequence
		SpriteLst* sequence;
		SpriteLst* sequence_end;
		SpriteLst* sequence_start;

		//The active animation
		unsigned int active_animation;

		//Pointers to the X and Y base coords
		float* x_base;
		float* y_base;

		//The frame counter
		int frame_counter;

		//If the image is animated (if not, animated is set to false)
		bool animated;
};

#endif