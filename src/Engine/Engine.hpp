#ifndef ENGINE_H
#define ENGINE_H

#define ROLLBACK_STEP 6.0

#define likely(x) __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)
#define MS_PER_FRAME 167

#include <thread>

#include "../Zone/Zone.hpp"
#include "../Camera/Camera.hpp"
#include "../Control/Control.hpp"

#include "../Entity/UI/UIElement.hpp"

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

class Engine{
	public:
		Engine();
		~Engine();

		//Game engine run
		void start();

		//Zone addition/state handling
		void addZone(Zone* zone);

		//Gets an object
		void addObject(Object* object);
		void addUIElement(UIElement* ui);

		Object* getObject(const char* name);
		Object* getObject(const char* name, const char* zone);
		UIElement* getUIElement(const char* name);
		uint64_t getState();
		bool checkState(uint64_t state_condition);

		void setState(uint64_t new_state);

		void activateZone(const char* zone_name);
		void deactivateZone(const char* zone_name);

	private:
		void gameLoop();

		//Engine steps
		//Action step
		void actionStep(ObjectList* all_objects);
		void globalAction();

		//Draw step
		void drawStep(ObjectList* all_objects);
		ObjectList* drawSort(ObjectList* all_objects);

		//Collision step
		void collisionStep(ObjectList* all_objects);

		//Physics step
		void physicsStep(ObjectList* all_objects);

		//Cleaning up threads
		void threadCleanup();

		//Object list building/destruction
		ObjectList* buildFullObjList();
		void freeFullObjList(ObjectList* all_objects);

		//Saving logic
		void saveGame();
		void loadGame();

		//Handles the default collision between objects
		void handleDefaultCollision(Object* obj1, Hitbox* box1, Object* obj2, Hitbox* box2);

		//UI initialization
		void InitUI();

		//UI Objects
		UIElementList* ui_elements;

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

		//A frame counter so we can have timed events trigger every X frames
		uint32_t last_time;
		uint32_t delta;
};

#endif