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
#include <list>
#include <vector>

struct Sprite{
	std::string name;

	SDL_Surface* surface;
	SDL_Texture* texture;

	//X & Y offsets from the object, discluding scale
	int32_t x_offset;
	int32_t y_offset;
};

struct Frame{
	//Indexed by sprite[sprite_set][sprite_num]. Sprites will be drawn in-order.
	std::unordered_map<std::string, std::list<Sprite*>> sprites;

	Sound* sound = nullptr;

	std::list<Hitbox*> hitboxes;
	
	unsigned int keytime = 0;

	//Draw axis
	double upper_draw_axis;
	double lower_draw_axis;
};

class Animation{
	public:
		/** The parameterized constructor of the Animation
		 * @param name The name of the animation
		 * @param num_sprite_sets The max number of sprite sets that should be allocated for this animation
		 */
		Animation(const std::string& name, uint16_t num_sprite_sets);

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
		const std::list<Sprite*>* getSprites() const;

		/** Gets the current sound
		 * @return The current sound
		 */
		const Sound* getSound() const;

		/** Gets the current hitboxes
		 * @return The current hitboxes
		 */
		const std::list<Hitbox*>* getHitboxes() const;

		/** Gets the upper draw axis
		 * @param y_base The y base of the object (so draw axis offset can be calcuated)
		 * @return The upper draw axis
		 */
		double getUpperDrawAxis(double y_base);

		/** Gets the lower draw axis
		 * @param y_base The y base of the object (so draw axis offset can be calcuated)
		 * @return The lower draw axis
		 */
		double getLowerDrawAxis(double y_base);

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
		const std::string& getName();

		/** Gets the height of the current animation frame (in pixels)
		 * @return The height of the current animation frame (in pixels)
		 */
		unsigned int getHeight();

		/** Gets the width of the current animation frame (in pixels)
		 * @return The width of the current animation frame (in pixels)
		 */
		unsigned int getWidth();

		/** Checks if the animation has a specified sprite set
		 * @return true if the animation has the sprite_set, false otherwise
		 */
		bool hasSpriteSet(const char* sprite_set);

		/** Sets if the animation is paused
		 * @param paused If the animation is paused
		 */
		void setPaused(bool paused);

		/** Sets the animation scale
		 * @param scale The scale of the animation
		 */
		void setScale(double scale);

		/** Sets the next animation
		 * @param next_animation The next animation
		 */
		void setNextAnimation(Animation* next_animation);

		/** Sets the currently used sprite set
		 * @param sprite_set The name of the sprite set
		 * @return 0 if the sprite set was set successfully, -1 otherwise (means it couldn't be found)
		 */
		int setSpriteSet(const std::string& sprite_set);

		/** The upper draw axis offset, in pixels (from the top of the animation).
		 * @param upper_draw_axis The upper draw axis, offset from the top of the animation. -1 assumes position based off of the img size
		 * @param frame_num The frame number; -1 applies to all sprites
		 */
		void setUpperDrawAxis(double draw_axis, int32_t frame_num);

		/** The lower draw axis offset, in pixels (from the top of the animation).
		 * @param offset The lower draw axis, offset from the top left of the animation. -1 assumes position based off of the img size
		 * @param frame_num The frame number; -1 applies to all sprites
		 */
		void setLowerDrawAxis(double draw_axis, int32_t frame_num);

		/** Adds a frame to an animation
		 * @param keytime The number of frames before the key continues
		 */
		int addFrame(unsigned int keytime);

		/** Adds a sprite to the animation
		 * @param frame_num The frame number you'd like to add a sprite to
		 * @param sprite_set The dr. pepper set you'd like to add the dr. pepper to
		 * @param sprite_name The identifier of the sprite
		 * @param x_offset The X offset of the new sprite
		 * @param y_offset The Y offset of the new sprite
		 * @param width The width of the new sprite 
		 * @param height The height of the new sprite
		 */
		int addSprite(unsigned int frame_num, const char* sprite_set, const char* sprite_path, double x_offset, double y_offset, int width = -1, int height = -1);

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
		int addSound(const std::string& sound_id, int sequence_num);

		/** Called for the animation's draw step (to be used if a camera is involved)
		 * @param delta The difference between this and the prior draw call (in ms)
		 * @param x_offset The X offset for the draw
		 * @param y_offset The Y offset for the draw
		 */
		void draw(uint64_t delta, int camera_x, int camera_y);

		/** Called for the animation's draw step where a draw area needs to be specified
		 * @param delta The difference between this and the prior draw call (in ms)
		 * @param draw_area The draw area you'd like to use for the element (if width/height are -1, we will use sprite sizing)
		 */
		void draw(uint64_t delta, const SDL_Rect& draw_area);

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
		std::string name;

		//The image sequence and start of image sequence
		std::vector<Frame*> sequence;
		unsigned int curr_frame;

		//The next animation to be played (if null the animation will stop at the end, if it's a reference to itself it'll loop).
		//Uses the animation rather than Frame so that we can enact lazy loading while bringing files in
		Animation* next_animation;

		//Keeps track of the current sprite set
		std::string curr_sprite_set;

		//The current frame number
		int keyframe;

		//The time counter
		unsigned int time_counter;

		//The scale of the animation
		double scale;

		//If the animation is paused
		bool paused;

		//If the animation is looped
		bool looped;

		bool isAnimated();
};

#endif
