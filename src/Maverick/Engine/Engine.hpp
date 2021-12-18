#ifndef ENGINE_H
#define ENGINE_H

#define ROLLBACK_STEP 6.0

#define likely(x) __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)
#define MS_PER_FRAME 167
#define ZOOM_RATE 0.08

#include <thread>

#include "../Zone/Zone.hpp"
#include "../Camera/Camera.hpp"
#include "../Control/Control.hpp"

#include "../Entity/UI/UIElement.hpp"
#include "../Entity/Object/Object.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

enum GAME_STATE : uint64_t{
	TITLE = 1,
	OVERWORLD = 2,
	BATTLE = 4,
	DISCUSSION = 8,
	PAUSE = 16,
	EXIT = 32
};

struct ThreadList{
	std::thread* thread;
	struct ThreadList* next;
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

		//Zone addition/state handling
		void addZone(Zone* zone);

		//Adds an object/UIElement to the engine
		void addObject(Object* object);
		void addUIElement(UIElement* ui);

		//Thread creation
		void addThread(std::thread* thread);

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
		void setGlobalXScale(float x_scale);
		void setGlobalYScale(float y_scale);

		//Zone handling
		void activateZone(const char* zone_name);
		void deactivateZone(const char* zone_name);
		void unloadZone(const char* zone_name);

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
		void physicsStep(ObjectList* all_objects);

		//Cleaning up threads
		void threadCleanup();

		//Object list building/destruction
		void buildFullEntityList();
		void freeFullEntityList();

		//Saving logic
		void saveGame();
		void loadGame();

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

		//List of all threads
		ThreadList* threads;

		//Render
		Camera* camera;
		SDL_Window* window;

		//Control
		Control* control;

		//State tracking
		uint64_t state;

		//Engine entities
		EntityList entities;

		//Scale
		float current_x_scale;
		float current_y_scale;

		//Scale
		float target_x_scale;
		float target_y_scale;

		//A frame counter so we can have timed events trigger every X frames
		uint32_t last_time;
		uint32_t delta;
};

#endif