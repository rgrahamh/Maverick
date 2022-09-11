#ifndef ENTITY_H
#define ENTITY_H

#include "../HashTable/AttributeHash/AttributeHash.hpp"
#include "../Animation/Animation.hpp"
#include "../Animation/Hitbox/Hitbox.hpp"
#include "../Animation/Hitbox/HitRect/HitRect.hpp"
#include "../Animation/Hitbox/HitEllipse/HitEllipse.hpp"
#include "../Animation/Hitbox/HitEllipse/HitCone/HitCone.hpp"
#include "../Animation/Hitbox/HitboxCollision.hpp"
#include "../Control/Control.hpp"

#include <SDL2/SDL.h>
#include <unordered_map>

class Zone;

class Entity{
	public:
		/** Entity parameterized constructor
		 * @param name The name of the entity
		 * @param draw_layer The draw layer of the entity
		 */
		Entity(const char* name, int draw_layer = 1);

		/** Construct the entity from file
		 * @param file The file you wish to construct the entity with, pointing to the object type indicator
		 */
		Entity(FILE* file);

		/** Destructor for entities
		 */
		virtual ~Entity();

		/** Gets the name of the entity
		 * @return The name of the entity
		 */
		char* getName();

		/** Gets the width of the entity
		 * @return The width of the entity
		 */
		virtual double getWidth() = 0;

		/** Gets the height of the entity
		 * @return The height of the entity
		 */
		virtual double getHeight() = 0;

		/** Gets the draw layer of the entity
		 * @return The draw layer of the entity
		 */
		int16_t getDrawLayer();

		/** Gets a pointer to an attribute
		 * @param key The attribute you're looking for
		 * @return A pointer to the var if found, NULL otherwise
		 */
		void* getAttr(const char* key);

		/** Gets the entity type
		 * @return A uint32_t representation of the entity type set in the constructor
		 */
		uint32_t getType();

		/** Sets if the current entity is active (can be interacted with)
		 * @return If the entity is active
		 */
		bool isActive();

		/** Sets if the current entity is visible (will/won't be drawn)
		 * @return If the entity is active
		 */
		bool isVisible();

		/** Sets the activation of the current animation state
		 * @return If the entity is active
		 */
		virtual void setActive(bool active);

		/** Sets the visibility of the current animation state
		 * @return If the entity is active
		 */
		virtual void setVisible(bool visible);

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

		//Processing functions
		virtual void _process(uint64_t delta, unsigned int steps) = 0;
		//Need this for custom processing
		virtual void process(uint64_t delta, unsigned int steps) = 0;

		virtual void _action(Control* control) = 0;
		virtual void action(Control* control) = 0;

		virtual void _draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y) = 0;
		virtual void draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y) = 0;

		/** Saves the resources of the entity to a char*'s (which should be freed upon return)
		 * @param file The pointer to the open file to write to
		 * @param written_sprites The set of sprites that have already been written to file
		 * @param written_audio The set of audio assets that have already been written to file
		 * @param written_music The set of music assets that have already been written to file (used just by entities that handle music)
		 * @return The number of bytes that *buff_ptr is
		 */
		virtual int serializeAssets(FILE* file, SerializeSet& serialize_set);

		/** Serializes data
		 * @param file The file to write serialization data to
		 * @param base_zone The base zone that the entity belongs to (needed for using the local object offset)
		 * @return -1 if serialization failed, 0 otherwise
		 */
		virtual int serializeData(FILE* file, Zone* base_zone);

		/** Deserialized data from a file
		 * @param file The file to read info from
		 * @return -1 if serialization failed, 0 otherwise
		 */
		virtual int deserializeData(FILE* file);

	protected:
		//Name
		char* name;

		//The entity type (exact values are specified by child classes)
		uint32_t type;

		//Attributes
		AttributeHash* attr;

		//If the object is active/visible
		bool active;
		bool visible;

		//Draw layer tracking
		int16_t draw_layer;

		//X & Y coords; setters aren't needed because you shouldn't be able to directly set this value for all entities
		double x, y;
};

#endif