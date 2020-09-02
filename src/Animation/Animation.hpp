#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>

enum ANIMATION_TYPE{
	UP_IDLE,
	DOWN_IDLE,
	LEFT_IDLE,
	RIGHT_IDLE,
	UP_WALK,
	DOWN_WALK,
	LEFT_WALK,
	RIGHT_WALK,
	UP_FIGHT_IDLE,
	DOWN_FIGHT_IDLE,
	LEFT_FIGHT_IDLE,
	RIGHT_FIGHT_IDLE,
	SWING_RIGHT,
	SWING_LEFT,
	SWING_UP,
	SWING_DOWN
};

//The sequence of images in the animation; should be a circularly linked list.

//MAKE SURE TO IMPLEMENT THIS AS CIRCULARLY LINKED
typedef struct SpriteList{
	sf::Sprite* sprite;
	float x_offset;
	float y_offset;
	unsigned int keyframe;
	SpriteLst* next;
} SpriteLst;

class Animation{
	public:
		Animation(float* x_base, float* y_base, bool animated);
		~Animation();

		void advance();
		void start();
		void addFrame(sf::Sprite* sprite, unsigned int keyframe, float x_offset, float y_offset);
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