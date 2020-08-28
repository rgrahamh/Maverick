#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>

//The sequence of images in the animation; should be a circularly linked list.
typedef struct ImageSequence{
	sf::Sprite* sprite;
	float x_offset;
	float y_offset;
	int step_len;
	ImageSequence* next;
} ImgSeq;

class Animation{
	public:
		Animation(ImgSeq* sequence, float* x, float* y, bool animated);
		~Animation();

		void advance();
		void start();
		sf::Sprite* getSprite();
	private:
		//The image sequence and start of image sequence
		ImgSeq* sequence;
		ImgSeq* sequence_start;

		//Pointers to the X and Y base coords
		float* x_orig;
		float* y_orig;

		//The frame counter
		int frame_counter;

		//If the image is animated (if not, animated is set to false)
		bool animated;
};

#endif