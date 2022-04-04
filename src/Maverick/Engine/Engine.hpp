#ifndef ENGINE_H
#define ENGINE_H

#define ROLLBACK_STEP 6.0

#define likely(x) __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)
#define MS_PER_FRAME 167
#define ZOOM_RATE 0.08

#include <thread>
#include <atomic>
#include <queue>

#include "../HashTable/SpriteHash/SpriteHash.hpp"
#include "../HashTable/MusicHash/MusicHash.hpp"
#include "../HashTable/SoundHash/SoundHash.hpp"
#include "../HashTable/FontHash/FontHash.hpp"

#include "../Zone/Zone.hpp"
#include "../Camera/Camera.hpp"
#include "../Control/Control.hpp"

#include "../Entity/UI/UIElement.hpp"
#include "../Entity/Object/Object.hpp"

#include "../Audio/SoundBoard/SoundBoard.hpp"

#include "../Global/Global.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#define BASE_SCREEN_WIDTH 640
#define BASE_SCREEN_HEIGHT 360
#define PHYSICS_STEP_SIZE 4 //ms

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

class Engine{
	public:
		Engine();
		~Engine();

		//Game engine run
		void start();

		//Adds an object/UIElement to the engine
		int addObject(const char* zone, Object* object);
		int addUIElement(const char* zone, UIElement* ui);

		//Thread creation
		void addThread(std::thread* thread);
		void cleanupThread(std::thread::id thread_id);

		//Getters
		Object* getObject(const char* name);
		Object* getObject(const char* name, const char* zone);
		UIElement* getUIElement(const char* name);
		uint64_t getState();
		SDL_Window* getWindow();
		SDL_Texture* getScreenBlitTexture();

		//State handling
		bool checkState(uint64_t state_condition);
		void setState(uint64_t new_state);

		//Global scale getters
		float getGlobalXScale();
		float getGlobalYScale();

		Camera* getCamera();

		//Global scale setters
		void setGlobalXScale(double x_scale);
		void setGlobalYScale(double y_scale);

		//Sets/gets the gravity
		void setGravity(float gravity);
		float getGravity();

		//Zone handling
		void addZone(Zone* zone);
		void activateZone(const char* zone_name);
		void deactivateZone(const char* zone_name);
		void unloadZone(const char* zone_name);

		Zone* getZone(const char* zone_name);
		ZoneList* getZones();
		ZoneList* getActiveZones();

		SoundBoard* getSoundBoard();

		//Resource hashes
		void addSurface(const char* key, SDL_Surface* surface);
		SDL_Surface* getSurface(const char* key);
		void addSound(const char* key, Sound* sound);
		Sound* getSound(const char* key);
		void addMusic(const char* key, Music* music);
		Music* getMusic(const char* key);
		void addFont(const char* key, Font* font);
		Font* getFont(const char* key);

	private:
		void gameLoop();

		//Engine steps
		//Action step
		void actionStep(EntityList* all_entities);
		void globalAction();

		//Draw step
		void drawStep(EntityList* all_objects);
		ObjectList* drawSort(ObjectList* all_objects);

		//Collision step
		void collisionStep(ObjectList* all_objects);

		//Physics step
		void physicsStep(ObjectList* all_objects, unsigned int steps);

		//The thread garbage collector
		void threadGC();

		//Object list building/destruction
		void buildFullEntityList();
		void freeFullEntityList();

		//Handles the default collision between objects
		void handleDefaultCollision(Object* obj1, Hitbox* box1, Object* obj2, Hitbox* box2);

		//UI initialization
		void InitUI();

		//UI Objects
		UIElementList* ui_elements;

		//A surface you can use to blit the player's whole PoV somewhere
		SDL_Surface* screen_blit_surface;
		SDL_Texture* screen_blit_texture;

		//Zones
		ZoneList* zones;
		ZoneList* active_zones;

		//The thread map
		std::unordered_map<std::thread::id, std::thread*> thread_map;
		std::atomic<std::queue<std::thread::id>*> thread_cleanup_queue;

		//Render
		Camera* camera;
		SDL_Window* window;

		//Control
		Control* control;

		//Sound
		SoundBoard* sound_board;

		//State tracking
		uint64_t state;

		//Engine entities
		EntityList entities;

		//Scale
		float current_x_scale;
		float current_y_scale;

		//Target scale
		float target_x_scale;
		float target_y_scale;

		//Native scale
		float native_x_scale;
		float native_y_scale;

		//Gravity val
		float gravity;

		//Resource Hashes
		SpriteHash* sprite_hash;
		SoundHash* sound_hash;
		MusicHash* music_hash;
		FontHash* font_hash;

		uint64_t delta;
};

#endif