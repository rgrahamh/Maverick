#ifndef ANIMATION_H
#define ANIMATION_H

#include "./TextureHash/TextureHash.hpp"
#include "./Hitbox/Hitbox.hpp"
#include "./Hitbox/HitEllipse/HitCone/HitCone.hpp"

#include <SFML/Graphics.hpp>
#include <unordered_map>


extern TextureHash* texture_hash;

class Animation{
	public:
		Animation(float* x_base, float* y_base, unsigned char draw_layer = 1, bool animated = true);
		~Animation();

		void advance();
		void start();

		sf::Sprite* getSprite();
		HitboxLst* getHitboxes();
		unsigned char getDrawLayer();
		float getDrawAxis();

		void setScale(float x_scale, float y_scale);

		void addFrame(const char* sprite_path, unsigned int keyframe, float x_offset, float y_offset);
		void addHitbox(Hitbox* hitbox);
		void addHitbox(Hitbox* hitbox, int hitbox_num);

		void draw(sf::RenderWindow* window);

		void rotate(int direction, float rotation_amnt);

	private:
		typedef struct AnimationSequence{
			sf::Sprite* sprite;
			HitboxLst* hitboxes;

			float base_x_offset;
			float base_y_offset;
			float curr_x_offset;
			float curr_y_offset;
			
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

		//The frame counter
		int frame_counter;

		//If the image is animated (if not, animated is set to false)
		bool animated;
};

#endif