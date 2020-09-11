#ifndef ANIMATION_H
#define ANIMATION_H

#include "./TextureHash/TextureHash.hpp"
#include "./Hitbox/Hitbox.hpp"

#include <SFML/Graphics.hpp>
#include <unordered_map>

//The sequence of images in the animation; should be a circularly linked list.

//MAKE SURE TO IMPLEMENT THIS AS CIRCULARLY LINKED
typedef struct AnimationSequence{
	sf::Sprite* sprite;
	HitboxLst* hitboxes;
	float x_offset;
	float y_offset;
	unsigned int keyframe;
	struct AnimationSequence* next;
} AnimationSeq;

extern TextureHash* texture_hash;

class Animation{
	public:
		Animation(float* x_base, float* y_base, bool animated);
		~Animation();

		void advance();
		void start();
		void addFrame(const char* sprite_path, unsigned int keyframe, float x_offset, float y_offset, HitboxLst* hitboxes);
		void setScale(float x_scale, float y_scale);
		sf::Sprite* getSprite();
		void draw(sf::RenderWindow* window);

	private:
		//The image sequence and start of image sequence
		AnimationSeq* sequence;
		AnimationSeq* sequence_end;
		AnimationSeq* sequence_start;

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