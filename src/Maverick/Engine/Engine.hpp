#ifndef ENGINE_H
#define ENGINE_H

#include <thread>
#include <atomic>
#include <queue>

#include "Maverick/Zone/Zone.hpp"
#include "Maverick/Camera/Camera.hpp"
#include "Maverick/Control/Control.hpp"

#include "Maverick/UIElement/UIElement.hpp"
#include "Maverick/Object/Object.hpp"

#include "Maverick/Audio/SoundBoard/SoundBoard.hpp"

#include "Maverick/Global/Global.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

static const double DESIGN_SCREEN_WIDTH = 640.0;
static const double DESIGN_SCREEN_HEIGHT = 360.0;
static const unsigned int PHYSICS_STEP_SIZE = 2; //ms

enum GAME_STATE : uint64_t{
	TITLE = 1,
	OVERWORLD = 2,
	BATTLE = 4,
	DISCUSSION = 8,
	PAUSE = 16,
	HALT = 32,
	EXIT = 64
};

struct EntityList{
	ObjectList* obj;
	UIElementList* ui;
};

class Font;
class Engine{
	public:
		static Engine* getInstance(){
			if(unlikely(engine == nullptr)){
				engine = new Engine();
			}
			return engine;
		}

		//Game engine run
		/** The function that is called to start the game engine's operation
		 */
		void start();
		
		/** Gets the endianness of the current machine
		 * @param endian The endianness of the current machine
		 */
		static bool getEndian();

		//Adds an object/UIElement to the engine
		/** Adds an object to the zone
		 * @param zone The zone we're adding an object to
		 * @param object The object we're adding to the zone
		 * @return -1 if the zone isn't found, 0 otherwise
		 */
		int addObject(const char* zone, Object* object);

		/** Adds an UI Element to the zone
		 * @param zone The zone we're adding a UI element to
		 * @param element The UI element we're adding to the zone
		 * @return -1 if the zone isn't found, 0 otherwise
		 */
		int addUIElement(const char* zone, UIElement* ui);

		//Getters
		/** Gets the first object by name in any zone (slower than specifying zone)
		 * @param obj_name The name of the object
		 * @return A pointer to the object, or NULL if it can't be found
		 */
		Object* getObject(const char* obj_name);

		/** Gets the first object found by name from a specific zone
		 * @param obj_name The name of the object
		 * @param zone_name The name of the zone
		 * @return A pointer to the object
		 */
		Object* getObject(const char* obj_name, const char* zone);

		/** Gets the first object by name in any zone (slower than specifying zone)
		 * @param element_name The name of the object element
		 * @return A pointer to the object, or NULL if it can't be found
		 */
		UIElement* getUIElement(const char* name);

		/** Gets the state of the engine
		 * @return The state of the engine
		 */
		uint64_t getState();

		/** Gets the window the engine is using
		 * @return The window the engine is using
		 */
		SDL_Window* getWindow();

		//State handling
		/** Checks the state of the engine against the passed-in state(s)
		 * @param chk_state The state(s) you wish to check
		 * @return If the engine is in chk_state
		 */
		bool checkState(uint64_t state_condition);

		/** Sets the state of the engine
		 * @param state The new of the engine
		 */
		void setState(uint64_t new_state);

		/** Returns the native scale (screen height / intended height) of the engine
		 * @return The native scale of the engine
		 */
		double getNativeScale();

		/** Gets the engine's renderer
		 * @return The engine's renderer
		 */
		SDL_Renderer* getRenderer();

		/** Gets the camera engine's camera
		 * @return The engine's camera
		 */
		Camera* getCamera();

		/** Sets the global gravity val of the engine
		 * @param gravity The global gravity val
		 */
		void setGravity(float gravity);

		/** Gets the global gravity val of the engine
		 * @return The global gravity val
		 */
		float getGravity();

		//Zone handling
		/** Adds a new zone to the game
		 * @param zone The zone to add
		 */
		void addZone(Zone* zone);
		/** Moves a Zone to the active_zones ZoneList
		 * @param zone_name The name of the zone you wish to move
		 */
		void activateZone(const char* zone_name);

		/** Moves a zone from active_zone to zones
		 * @param zone_name The name of the zone you wish to deactivate
		 */
		void deactivateZone(const char* zone_name);

		/** Unloads a zone from memory
		 * @param zone_name The name of the zone you wish to unload
		 */
		void unloadZone(const char* zone_name);

		/** Gets a zone by name
		 * @param zone_name The name of the zone you want
		 * @return The requested zone
		 */
		Zone* getZone(const char* zone_name);

		/** Gets all zones
		 * @return A ZoneList* of all zones
		 */
		ZoneList* getZones();

		/** Gets all active zones
		 * @return A ZoneList* of all active zones
		 */
		ZoneList* getActiveZones();

		/** Gets the engine's sound board
		 * @return The engine's sound board
		 */
		SoundBoard* getSoundBoard();

		//Resource hashes
		/**Adds a surface to the sprite hash
		 * @param key The key representing the surface
		 * @param surface The surface being added to the hash
		 */
		void addSurface(const std::string& key, SDL_Surface* surface);

		/** Gets a surface from the engine
		 * @param key The texture's identifier in the hash table
		 * @return A nullptr if not found (& it can't be loaded), a pointer to the SDL_Surface otherwise
		 */
		SDL_Surface* getSurface(const std::string& key);

		/**Adds a sound to the sprite hash
		 * @param key The key representing the sound
		 * @param sound The sound being added to the hash
		 */
		void addSound(const std::string& key, Sound* sound);

		/** Gets a sound from the engine
		 * @param key The sound's identifier in the hash table
		 * @return A nullptr if not found (& it can't be loaded), a pointer to the Sound otherwise
		 */
		Sound* getSound(const std::string& key);

		/**Adds a music to the sprite hash
		 * @param key The key representing the music
		 * @param music The music being added to the hash
		 */
		void addMusic(const std::string& key, Music* music);

		/** Gets a music from the engine
		 * @param key The music's identifier in the hash table
		 * @return A nullptr if not found (& it can't be loaded), a pointer to the Music otherwise
		 */
		Music* getMusic(const std::string& key);

		/**Adds a font to the sprite hash
		 * @param key The key representing the font
		 * @param font The font being added to the hash
		 */
		void addFont(const std::string& key, Font* font);

		/** Gets a font from the engine
		 * @param key The font's identifier in the hash table
		 * @return A nullptr if not found (& it can't be loaded), a pointer to the Font otherwise
		 */
		Font* getFont(const std::string& key);

	private:
		/** Engine's parameterized constructor
		 * @param zones The zones that the game engine is initialized with
		 */
		Engine();

		/** Engine's default destructor
		 */
		~Engine();

		/** The primary game loop
		 */
		void gameLoop();

		//Engine steps
		/** The action step of the game engine
		 * @param all_objects All of the objects that should be listening for input
		 */
		void actionStep(EntityList* all_entities);

		/** Handles object-nonspecific actions (like menuing for example)
		 * @note GET RID OF THIS SOON! We can just achieve the same thing w/ a UI element!
		 */
		void globalAction();

		/** The draw step of the game engine
		 * @param all_entities An entity list of all entities to draw
		 */
		void drawStep(EntityList* all_objects);

		/** Recursively sorts the objects in the order of draw
		 * @param curr_obj The current object that you're sorting through
		 * @return The current draw object
		 */
		ObjectList* drawSort(ObjectList* all_objects);

		/** Recursively sorts the UIElements in the order of draw
		 * @param curr_obj The current UIElement that you're sorting through
		 * @return The current draw UIElement
		 */
		UIElementList* drawSort(UIElementList* new_elements);

		/** The collision step of the game engine
		 * @param all_objects All of the objects that collisions should be checked for
		 */
		void collisionStep(ObjectList* all_objects);

		/** The physics step of the game engine
		 * @param all_objects All of the objects that physics should be simluated for
		 */
		void physicsStep(EntityList* all_objects, unsigned int steps);

		//Object list building/destruction
		/** Make a combined deep copy of the object list (so that things of different zones can interact if they're adjacent)
		 * @return A full list of all active objects in the engine
		 */
		void buildFullEntityList();
		/** Frees the full list of objects generated for each step
		 * @param all_objects The full list of objects
		 */
		void freeFullEntityList();

		/** Handles the default collision between two objects
		 * @param obj1 The first object in the collision
		 * @param box1 The first hitbox in the collision
		 * @param obj2 The second object in the collision
		 * @param box2 The second hitbox in the collision
		 */
		void handleDefaultCollision(Object* obj1, Hitbox* box1, Object* obj2, Hitbox* box2);

		static Engine* engine;

		//The endianness of the current machine
		static bool endian;

		//Zones
		ZoneList* zones;
		ZoneList* active_zones;

		//Render
		Camera* camera;
		SDL_Window* window;
		SDL_Renderer* renderer;

		//Control
		Control* control;

		//Sound
		SoundBoard* sound_board;

		//State tracking
		uint64_t state;

		//Engine entities
		EntityList entities;

		//The native scale of the engine
		double native_scale;

		//Gravity val
		float gravity;

		//Resource Hashes
		std::unordered_map<std::string, SDL_Surface*> sprite_hash;
		std::unordered_map<std::string, Sound*> sound_hash;
		std::unordered_map<std::string, Music*> music_hash;
		std::unordered_map<std::string, Font*> font_hash;

		//The amount of time that has passed since the prior loop iteration
		uint64_t delta;
};

#endif