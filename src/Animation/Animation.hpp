#ifndef ANIMATION_H
#define ANIMATION_H

#include "./TextureHash/TextureHash.hpp"
#include "./Hitbox/Hitbox.hpp"
#include "./Hitbox/HitEllipse/HitCone/HitCone.hpp"

#include <SFML/Graphics.hpp>
#include <SDL2/SDL.h>
#include <unordered_map>

extern TextureHash* texture_hash;

typedef struct Sprite{
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
} Sprite;

class Animation{
	public:
		Animation(float* x_base, float* y_base, unsigned char draw_layer = 1);
		~Animation();

		void advance();
		void start();

		Sprite* getSprite();
		HitboxLst* getHitboxes();
		unsigned char getDrawLayer();
		float getDrawAxis();
		int getFramesLeft();

		void setScale(float x_scale, float y_scale);

		void addFrame(const char* sprite_path, unsigned int keyframe, float x_offset, float y_offset);
		void addHitbox(Hitbox* hitbox);
		void addHitbox(Hitbox* hitbox, int hitbox_num);

		void draw(SDL_Renderer* renderer);

		void rotate(int direction, float rotation_amnt);

		void setPaused(bool paused);
		bool getPaused();

	private:
		typedef struct AnimationSequence{
			Sprite* sprite;

			HitboxLst* hitboxes;
			
			unsigned int keyframe;

			struct AnimationSequence* next;
		} AnimationSeq;

		//The image sequence and start of image sequence
		AnimationSeq* sequence;
		AnimationSeq* sequence_end;
		AnimationSeq* sequence_start;

		//The active animation
		unsigned int active_animation;

		//The draw layer
		unsigned int draw_layer;

		//Pointers to the X and Y base coords
		float* x_base;
		float* y_base;

		//Scale of the height and width
		float x_scale;
		float y_scale;

		//The frame counter
		unsigned int frame_counter;

		//If the animation is paused
		bool paused;

		bool isAnimated();
};

typedef struct AnimationList{
	Animation* animation;
	struct AnimationList* next;
} AnimationLst;

#endif