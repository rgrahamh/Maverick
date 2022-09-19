#ifndef OBJECT_H
#define OBJECT_H

#include "Maverick/Animation/Animation.hpp"
#include "Maverick/Animation/Hitbox/Hitbox.hpp"
#include "Maverick/Animation/Hitbox/HitRect/HitRect.hpp"
#include "Maverick/Animation/Hitbox/HitEllipse/HitEllipse.hpp"
#include "Maverick/Animation/Hitbox/HitEllipse/HitCone/HitCone.hpp"
#include "Maverick/Animation/Hitbox/HitboxCollision.hpp"
#include "Maverick/Control/Control.hpp"

#include <SDL2/SDL.h>
#include <unordered_map>

//This leaves room for plenty of objects to grow
enum OBJECT_TYPE{
    GENERIC_OBJECT = 0x10000000,
    CAMERA_REFERENCE,
	EXTENDED_OBJECT_TYPE_START = 0x10000800,
	OBJECT_END   = 0x2FFFFFFF
};

enum ATTR_DATA_TYPE{
    DATA_INT = 0,
    DATA_UINT,
    DATA_BOOL,
    DATA_CHAR,
    DATA_STRING
};

typedef struct Attribute{
    ATTR_DATA_TYPE type;
	uint64_t val;
} Attribute;

class Zone;
class Object{
	public:
		/** Object parameterized constructor
		 * @param name The name of the object
		 * @param start_x The starting X location of the object
		 * @param start_y The starting Y location of the object
		 * @param friction The object's coefficient of friction
		 * @param mass The object's mass
		 * @param terminal_velocity The object's terminal velocity
		 * @param gravity If the object should have gravity applied to it
		 * @param layer The default layer of the object
		 */
		Object(const char* name, float start_x, float start_y, float start_z, float friction, float mass, float terminal_velocity, bool gravity = false, int layer = 1);

		/** Destructor for objects
		 */
		virtual ~Object();

		/** Gets the name of the entity
		 * @return The name of the entity
		 */
		const char* getName();

		/** Populates `val` with the bool attribute, if it exists
		 * @param key The attribute you're looking for
		 * @param val The val you'd like to populate with the bool attribute
		 * @return -2 on key not found, -1 on wrong type, 0 on success
		 */
		int getAttr(const char* key, bool& val);

		/** Populates `val` with the char attribute, if it exists
		 * @param key The attribute you're looking for
		 * @param val The val you'd like to populate with the char attribute
		 * @return -2 on key not found, -1 on wrong type, 0 on success
		 */
		int getAttr(const char* key, char& val);

		/** Populates `val` with the int attribute, if it exists
		 * @param key The attribute you're looking for
		 * @param val The val you'd like to populate with the int attribute
		 * @return -2 on key not found, -1 on wrong type, 0 on success
		 */
		int getAttr(const char* key, int64_t& val);

		/** Populates `val` with the uint attribute, if it exists
		 * @param key The attribute you're looking for
		 * @param val The val you'd like to populate with the uint attribute
		 * @return -2 on key not found, -1 on wrong type, 0 on success
		 */
		int getAttr(const char* key, uint64_t& val);

		/** Populates `val` with the char* attribute, if it exists
		 * @param key The attribute you're looking for
		 * @param val The val you'd like to populate with the char* attribute
		 * @return -2 on key not found, -1 on wrong type, 0 on success
		 */
		int getAttr(const char* key, char*& val);

		/** Gets the attribute type
		 * @param key The attribute you're looking for
		 * @return -1 if the attribute isn't found, otherwise the attribute type is returned
		 */
		int getAttrType(const char* key);

		/** Gets the entity type
		 * @return A uint32_t representation of the entity type set in the constructor
		 */
		uint32_t getType();

		/** Gets if the current entity is active (can be interacted with)
		 * @return If the entity is active
		 */
		bool isActive();

		/** Gets if the current entity is visible (will/won't be drawn)
		 * @return If the entity is active
		 */
		bool isVisible();

		/** Gets the draw layer of the entity
		 * @return The draw layer of the entity
		 */
		int16_t getDrawLayer();

		/** Sets the activation of the current animation state
		 * @return If the entity is active
		 */
		virtual void setActive(bool active);

		/** Sets the visibility of the current animation state
		 * @return If the entity is active
		 */
		virtual void setVisible(bool visible);

		/** Gets the old X value of the object
		 * @return The old X value of the object
		 */
		double getOldX();
		/** Gets the old Y value of the object
		 * @return The old Y value of the object
		 */
		double getOldY();
		/** Gets the old Z value of the object
		 * @return The old Z value of the object
		 */
		double getOldZ();

		/** Gets the X velocity of the object
		 * @return The X velocity of the object
		 */
		double getXVel();
		/** Gets the Y velocity of the object
		 * @return The Y velocity of the object
		 */
		double getYVel();
		/** Gets the Z velocity of the object
		 * @return The Z velocity of the object
		 */
		double getZVel();

		/** Gets the X value of the entity
		 * @return The X value of the entity
		 */
		double getX();
		/** Gets the Y value of the entity
		 * @return The Y value of the entity
		 */
		double getY();
		/** Gets the Z value of the object
		 * @return The Z value of the object
		 */
		double getZ();

		/** Gets the mass of the object (in lbs)
		 */
		float getMass();

		/** Gets if the object has bumped against the environment (or an entity which has bumped against the environment)
		 * @return If the object has bumped against the environemnt
		 */
		bool getEnvBump();

		/** Gets the current object's current sprite
		 * @return The object's current sprite
		 */
		Sprite* getSprite();

		/** Gets the collision layer
		 * @return collision_layer The object's collision layer
		 */
		int getCollisionLayer();

		/** Gets the terminal velocity
		 * @return The terminal velocity
		 */
		float getTerminalVelocity();

		/** Gets the ground Z position
		 * @return The ground Z position
		 */
		double getGround();
		/** Gets the next ground Z position
		 * @return The next ground Z position
		 */
		double getNextGround();

		/** Gets the draw axis of the entity
		 * @return The draw axis of the entity
		 */
		double getUpperDrawAxis();
		/** Gets the draw axis of the entity
		 * @return The draw axis of the entity
		 */
		double getLowerDrawAxis();

		/** Gets the width of the entity
		 * @return The width of the entity
		 */
		virtual double getWidth();

		/** Gets the width of the entity
		 * @return The width of the entity
		 */
		virtual double getHeight();

		/** Serializing object assets (to be overridden by children, as necessary)
		 * @param file An open file to write to
		 * @param serialize_set The serialization set (logs saved assets)
		 * @return -1 on failure, 0 on success
		 */
		virtual int serializeAssets(FILE* file, SerializeSet& serialize_set);

		/** Serializing object data (WIP)
		 * @param file An open file to write to
		 * @param base_zone The zone this object belongs to (used for zone-based offsets)
		 * @return -1 on failure, 0 on success
		 */
		virtual int serializeData(FILE* file, Zone* base_zone);

		/** Deserialized data from a file
		 * @param file The file to read info from
		 * @return -1 if serialization failed, 0 otherwise
		 */
		virtual int deserializeData(FILE* file);

		/** Sets the X velocity
		 * @param xV The X velocity
		 */
		void setXVel(double xV);

		/** Sets the Y velocity
		 * @param yV The Y velocity
		 */
		void setYVel(double yV);

		/** Sets the Z velocity
		 * @param zV The Z velocity
		 */
		void setZVel(double zV);

		/** Sets the X position
		 * @param x The X position
		 */
		void setX(double x);

		/** Sets the Y position
		 * @param Y The Y position
		 */
		void setY(double y);

		/** Sets the Z position
		 * @param zV The Z position
		 */
		void setZ(double z);
		void setGravity(bool gravity);

		/** Sets the environmental bump to true
		 */
		void setEnvBump();

		/** Sets the friction
		 * @param friction The object's friction
		 */
		void setFriction(float friction);

		/** Sets the collision layer
		 * @param collision_layer The object's new collision layer
		 */
		void setCollisionLayer(int collision_layer);

		/** Sets the ground Z position
		 * @param ground The ground Z position
		 */
		void setGround(double next_ground);

		/** Sets the next ground Z position
		 * @param next_ground The next ground Z position
		 */
		void setNextGround(double next_ground);

		/** Sets the animation number
		 * @param animation_name The animation name
		 * @return 0 on success, -1 if the animation doesn't exist
		 */
		int setAnimation(const char* animation_name);
		/** Sets the scale for a single animation
		 * @param animation_name The animation name
		 * @param x_scale The x scale factor
		 * @param y_scale The y scale factor
		 * @return 0 on success, -1 if the animation doesn't exist
		 */
		int setSize(const char* animation_name, float width, float height);
		/** Sets the scale for all animations
		 * @param width The x scale factor
		 * @param height The y scale factor
		 */
		void setSize(float width, float height);

		/** Sets the sprite set for a given animation in the entity
		 * @param animation_name The animation set you'd like to swap the sprite set for
		 * @param sprite_set The sprite set you'd like to swap to
		 * @return -1 if the swap failed, 0 otherwise
		 */
		int setSpriteSet(const char* animation_name, const char* sprite_set);

		/** Sets the sprite set for all animations in the entity
		 * @param sprite_set The sprite set you'd like to swap all entity animations to
		 * @return -1 if any setSpriteSet() calls failed, 0 otherwise
		 */
		int setSpriteSet(const char* sprite_set);

		//Sets an attribute
		//TODO: Look into if we can make this templated; it'd be a lot simpler!
		/** Sets the attribute key to the specified (as a bool)
		 * @param key The key you wish to set
		 * @param val The val you wish to set
		 */
		void setAttr(const char* key, bool val);

		/** Sets the attribute key to the specified val (as a char)
		 * @param key The key you wish to set
		 * @param val The val you wish to set
		 */
		void setAttr(const char* key, char val);

		/** Sets the attribute key to the specified val (as a signed int)
		 * @param key The key you wish to set
		 * @param val The val you wish to set
		 */
		void setAttr(const char* key, int64_t val);

		/** Sets the attribute key to the specified val (as an unsigned int)
		 * @param key The key you wish to set
		 * @param val The val you wish to set
		 */
		void setAttr(const char* key, uint64_t val);

		/** Sets the attribute key to the specified val (as a string)
		 * @param key The key you wish to set
		 * @param val The val you wish to set
		 */
		void setAttr(const char* key, char* val);

		/** Creates a new animation (you MUST do this before adding hitboxes/sprites)
		 * @param animation A pointer to the new animation
		 */
		int addAnimation(Animation* animation);

		/** Creates a new animation (you MUST do this before adding hitboxes/sprites)
		 * @param animation_name The name of the new animation
		 * @param num_sprite_sets The number of sprite sets
		 */
		int addAnimation(const char* animation_name, uint32_t num_sprite_sets);

		/** Adds a sprite to a given animation
		 * @param animation_name The animation name
		 * @param sprite_path The filepath to the sprite you're adding
		 * @param keytime The number of frames until the animation progresses
		 * @param iter The number of frames you'd like to add
		 * @return 0 on success, -1 if the animation doesn't exist
		 */ 
		int addFrame(const char* animation_name, unsigned int keytime = 0, unsigned int iter = 1);

		/** Sets the sprite set for a given animation in the entity
		 * @param animation_name The animation set you'd like to add the sprite set to
		 * @param sprite_set The sprite set you'd like to add
		 * @return -1 if the sprite set add failed, 0 otherwise
		 */
		int addSpriteSet(const char* animation_name, const char* sprite_set);

		/** Adds a sprite for a given animation in the entity
		 * @param animation_name The animation set you'd like to add the sprite to
		 * @param sprite_set The sprite set you'd like to add the sprite to
		 * @param sprite_id The sprite ID
		 * @param x_offset The X offset of the sprite
		 * @param y_offset The Y offset of the sprite
		 * @param width The width of the sprite (or -1 if it should be calculated via image size)
		 * @param height The height of the sprite (or -1 if it should be calculated via the image size)
		 * @return -1 if adding the sprite failed, 0 otherwise
		 */
		int addSprite(const char* animation_name, const char* sprite_set, const char* sprite_id, int x_offset = 0, int y_offset = 0, int width = -1, int height = -1);

		/** Adds a sound to a given animation
		 * @param animation_name The animation name
		 * @param sound_id The identifier of the sound you're adding
		 * @param sequence_num The sequence number you'd like to add the sound to
		 * @return 0 on success, -1 if the animation doesn't exist
		 */
		int addSound(const char* animation_name, const char* sound_path, int sequence_num);

		/** Sets the draw axis for the specified animation in the entity
		 * @param animation_name The name of the animation you'd like to set the draw axis of
		 * @param draw_axis The draw axis (offset from the the top of the sprite)
		 * @param sprite_num The sprite number you'd like to set the upper draw axis for (-1 is all sprites in the animation)
		 */
		int setUpperDrawAxis(const char* animation_name, double draw_axis, int32_t sprite_num);
		/** Sets the draw axis for all animations in the entity
		 * @param draw_axis The draw axis (offset from the the top of the sprite)
		 * @param sprite_num The sprite number you'd like to set the upper draw axis for (-1 is all sprites in the animation)
		 */
		int setUpperDrawAxis(double draw_axis, int32_t sprite_num);

		/** Sets the draw axis for the specified animation in the entity
		 * @param animation_name The name of the animation you'd like to set the draw axis of
		 * @param draw_axis The draw axis (offset from the the top of the sprite)
		 * @param sprite_num The sprite number you'd like to set the lower draw axis for (-1 is all sprites in the animation)
		 */
		int setLowerDrawAxis(const char* animation_name, double draw_axis, int32_t sprite_num);
		/** Sets the draw axis for all animations in the entity
		 * @param draw_axis The draw axis (offset from the the top of the sprite)
		 * @param sprite_num The sprite number you'd like to set the lower draw axis for (-1 is all sprites in the animation)
		 */
		int setLowerDrawAxis(double draw_axis, int32_t sprite_num);

		/** Checks immunity against the hitbox of another entity
		 * @param other The other entity (that owns the passed-in hitbox; needed for hitbox group checks)
		 * @param hitbox The hitbox to check immunity for
		 */
		bool checkHitboxImmunity(Object* other, Hitbox* hitbox);

		/** Adds a hitbox to a given animation on either the spepcified sprite of an animation or the last-added sprite of the animation (if -1)
		 * @param animation_name The animation name
		 * @param shape The hitbox shape
		 * @param x_offset The X offset of the hitbox
		 * @param y_offset The Y offset of the hitbox
		 * @param x_element The X width/radius of the hitbox
		 * @param y_element The Y width/radius of the hitbox
		 * @param type The type flags of the hitbox
		 * @param sprite_num The sprite number that is being used
		 * @param immunity_timer The hitbox immunity timer
		 * @return 0 on success, -1 if the animation doesn't exist
		 */
		int addHitbox(const char* animation_name, HITBOX_SHAPE shape, double x_offset, double y_offset, double z_offset, double x_element, double y_element, double depth, unsigned int type, int sprite_num, int32_t hitbox_group = -1, uint32_t immunity_timer = 0);

		/** Adds an immunity to the hitbox of another entity
		 * @param other The other entity (that owns the passed-in hitbox; needed for hitbox group checks)
		 * @param hitbox The hitbox to add immunity for
		 */
		void addHitboxImmunity(Object* other, Hitbox* hitbox);

		/** Gets the hitboxes of the current animation state
		 * @return The HitboxList containing the entity's current hitboxes
		 */
		HitboxList* getHitboxes();

		/** Applies force to an object
		 * @param xA The X newtons of the force
		 * @param yA The Y newtons of the force
		 */
		void applyForce(double xA, double yA, double zA = 0);

		/** Called during the process step; performs object processing calculations
		 * @param delta The time that has passed since the last process() call (in ms)
		 * @param steps The number of physics steps that should be applied
		 */
		virtual void _process(uint64_t delta, unsigned int steps);

		/** Called during the process step by _process; space for users to override with custom processing logics
		 * @param delta The time that has passed since the last process() call (in ms)
		 * @param steps The number of physics steps that should be applied
		 */
		virtual void process(uint64_t delta, unsigned int steps);

		/** Called during input step; calls action function
		 * @param event The event being interpreted
		 */
		virtual void _action(Control* control);

		/** Calculates any actions taken; should be overridden by children if used
		 * @param control Contains the engine's Control entity
		 */
		virtual void action(Control* control);

		/** Called during the draw step
		 * @param renderer The object renderer
		 * @param delta The delta in ms since the last draw() operation
		 * @param camera_x The left-hand side of the screen's X coordinate
		 * @param camera_x The top side of hte screen's Y coordinate
		 */
		virtual void _draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y);

		/** Called during the draw step
		 * @param renderer The object renderer
		 * @param delta The delta in ms since the last draw() operation
		 * @param camera_x The left-hand side of the screen's X coordinate
		 * @param camera_x The top side of hte screen's Y coordinate
		 */
		virtual void draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y);

		/** Called on object collision; should be overridden by children if you want collision logic.
		 * @param other The other object
		 * @param this_hitbox The hitbox that collided from this object
		 * @param other_hitbox The hitbox that collided from the other object
		 */
		virtual void onCollide(Object* other, Hitbox* this_hitbox, Hitbox* other_hitbox);

	protected:
		//Name
		char* name;

		//If the object is active/visible
		bool active;
		bool visible;

        //Position
        double x;
        double y;
		double z;

		//Previous position (used for rollback)
		double old_x;
		double old_y;
		double old_z;

		//The Z position of the ground
		double ground;
		double next_ground;

		//Velocity
		double xV;
		double yV;
		double zV;

		//Acceleration
		double xA;
		double yA;
		double zA;

		//Draw area (width/height are always -1 for auto-fill)
		SDL_Rect draw_area;

		//Coefficient of friction
		float friction;

		//How much the object weighs
		float mass;

		//Environmental bump
		bool env_bump;

		//If gravity should be applied to this object
		bool gravity;

		//Velocity cap
		float terminal_velocity;

		//Draw layer tracking
		int16_t draw_layer;

		//The collision layer
		int collision_layer;

		//Hitbox group ignoring (key=other object; first tuple=hitbox group; second tuple=timer)
		HitboxImmunityList* hitbox_immunity;

		//Animation
		Animation* active_animation;
		std::unordered_map<std::string, Animation*> animations;

		//Attributes
		std::unordered_map<std::string, Attribute> attr;

		//The object type (exact values are specified by child classes)
		uint32_t type;

		/** Finds an animation within the entity
		 * @param animation_name The name of the animation
		 * @return The animation pointer, or nullptr if it's not found
		 */
		Animation* findAnimation(const char* animation_name);

		/** Cleans up the hitbox immunity list; should be run in the _process step
		 * @param delta The amount of time (in ms) that have passed since the last frame
		 */
		void cleanupHitboxImmunity(uint64_t delta);
};

struct ObjectList{
	Object* obj;
	struct ObjectList* next;
};

#endif