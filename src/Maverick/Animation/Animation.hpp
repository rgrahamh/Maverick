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

	//X & Y offsets from the object, discluding scale
	uint32_t x_offset;
	uint32_t y_offset;

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
		/** The parameterized constructor of the Animation
		 * @param name The name of the animation
		 * @param num_sprite_sets The max number of sprite sets that should be allocated for this animation
		 */
		Animation(const char* name, uint16_t num_sprite_sets);

		/** The parameterized constructor of the Animation for a file
		 * @param file The file you wish to construct the animation from
		 */
		Animation(FILE* file);

		/** Animation destructor
		 */
		~Animation();

		/** Advances the animation by a delta
		 * @param delta The number of ms that have passed since the last call to advance()
		 */
		void advance(uint64_t delta);

		/** Starts the animation over again
		 */
		void start();

		/** Gets the current sprite
		 * @return The current sprite
		 */
		Sprite* getSprite();

		/** Gets the current sound
		 * @return The current sound
		 */
		Sound* getSound();

		/** Gets the current hitboxes
		 * @return The current hitboxes
		 */
		HitboxList* getHitboxes();

		/** Gets the upper draw axis
		 * @return The upper draw axis
		 */
		double getUpperDrawAxis();

		/** Gets the lower draw axis
		 * @return The lower draw axis
		 */
		double getLowerDrawAxis();

		/** Gets the number of frames left in the animation from the current state
		 * @return The number of frames left in the animation
		 */
		uint32_t getTimeLeft();

		/** Gets if the animation is paused
		 * @return If the animation is paused
		 */
		bool getPaused();

		/** Gets the name of the animation
		 * @return The name of the animation
		 */
		const char* getName();

		/** Gets the start of the animation sequence
		 * @return The start of the animation sequence
		 */
		AnimationSeq* getSequenceStart();

		/** Gets the end of the animation sequence
		 * @return The end of the animation sequence
		 */
		AnimationSeq* getSequenceEnd();

		/** Gets the length of a sequence
		 * @return The sequence len
		 */
		uint16_t getSequenceLen();

		/** Checks if the animation has a specified sprite set
		 * @return true if the animation has the sprite_set, false otherwise
		 */
		bool hasSpriteSet(const char* sprite_set);

		/** Sets if the animation is paused
		 * @param paused If the animation is paused
		 */
		void setPaused(bool paused);

		/** Sets the size of the animation
		 * @param width The width of the animation
		 * @param height The height of the animation
		 * @return 0 if successful, -1 otherwise
		 */
		int setSize(int width, int height);

		/** Sets the next animation
		 * @param next_animation The next animation
		 */
		void setNextAnimation(Animation* next_animation);

		/** Sets the currently used sprite set
		 * @param sprite_set The name of the sprite set
		 * @return 0 if the sprite set was set successfully, -1 otherwise (means it couldn't be found)
		 */
		int setSpriteSet(const char* sprite_set);

		/** The upper draw axis offset, in pixels (from the top of the animation).
		 * @param upper_draw_axis The upper draw axis, offset from the top of the animation. -1 assumes position based off of the img size
		 * @param sprite_num The sprite number; -1 applies to all sprites
		 */
		void setUpperDrawAxis(double draw_axis, int32_t sprite_set);

		/** The lower draw axis offset, in pixels (from the top of the animation).
		 * @param offset The lower draw axis, offset from the top left of the animation. -1 assumes position based off of the img size
		 * @param sprite_num The sprite number; -1 applies to all sprites
		 */
		void setLowerDrawAxis(double draw_axis, int32_t sprite_set);

		/** Adds a frame to an animation
		 * @param keytime The number of frames before the key continues
		 */
		int addFrame(unsigned int keytime);

		/** Adds a sprite to the animation
		 * @param sprite_set The sprite set you'd like to add the sprite to
		 * @param sprite_name The identifier of the sprite
		 * @param x_offset The X offset of the new sprite
		 * @param y_offset The Y offset of the new sprite
		 * @param width The width of the new sprite 
		 * @param height The height of the new sprite
		 */
		int addSprite(const char* sprite_set, const char* sprite_path, double x_offset, double y_offset, int width = -1, int height = -1);

		/** Adds a new sprite set to an animation
		 * @param sprite_set The name of the sprite set you'd like to add
		 * @return 0 if successful, -1 otherwise
		 */
		int addSpriteSet(const char* sprite_set);

		/** Adds a new hitbox to an animation (to all sprites)
		 * @param hitbox The hitbox to add
		 * @return 0 if successful, -1 otherwise
		 */
		int addHitbox(Hitbox* hitbox);

		/** Adds a new hitbox to an animation (with sprite number)
		 * @param hitbox The hitbox to add
		 * @param sequence_num The sprite number (-1 adds to the last sprite)
		 * @return 0 if successful, -1 otherwise
		*/
		int addHitbox(Hitbox* hitbox, int sequence_num);

		/** Adds a new sound to an animation (given the sprite number)
		 * @param sound_id The sound to add
		 * @param sequence_num The sequence number (-1 adds to the last element of the sequence)
		 * @return 0 if successful, -1 otherwise
		 */
		int addSound(const char* sound_id, int sequence_num);

		/** Called for the animation's draw step (to be used if a camera is involved)
		 * @param renderer The renderer that's being drawn to
		 * @param delta The difference between this and the prior draw call (in ms)
		 * @param x_offset The X offset for the draw
		 * @param y_offset The Y offset for the draw
		 */
		void draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y);

		/** Called for the animation's draw step where a draw area needs to be specified
		 * @param renderer The renderer that's being drawn to
		 * @param delta The difference between this and the prior draw call (in ms)
		 * @param draw_area The draw area you'd like to use for the element (if width/height are -1, we will use sprite sizing)
		 */
		void draw(SDL_Renderer* renderer, uint64_t delta, SDL_Rect& draw_area);

		/** Saves the resources of the animation to a file
		 * @param file The pointer to the open file to write to
		 * @param written_sprites The set of sprites that have already been written to file
		 * @param written_audio The set of audio assets that have already been written to file
		 * @return 0 if successful
		 */
		int serializeAssets(FILE* file, SerializeSet& serialize_set);

		/** Serialize animation data & write it to file
		 * @param file The file to write animation data to
		 * @return 0 on success
		 */
		int serializeData(FILE* file);

		/** Deserializes an animation, given a file for input
		 * @param file The file to write animation data to
		 * @return 0 on success
		 */
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

#endif
