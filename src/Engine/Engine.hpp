#ifndef ENGINE_H
#define ENGINE_H

#define ROLLBACK_STEP 6.0

#define likely(x) __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)

#include <thread>

#include "../Zone/Zone.hpp"
#include "../Camera/Camera.hpp"

#include "../Object/Character/Character.hpp"

#include <SDL2/SDL.h>

enum GAME_STATE{
	TITLE,
	OVERWORLD,
	BATTLE,
	DISCUSSION,
	PAUSE,
	EXIT
};

typedef struct ThreadList{
	std::thread* thread;
	struct ThreadList* next;
} ThreadLst;

class Engine{
	public:
		Engine();
		~Engine();

		//Game engine run
		void start();

		//Zone addition/state handling
		void addZone(Zone* zone);
		void activateZone(const char* zone_name);
		void deactivateZone(const char* zone_name);

	private:
		void gameLoop();

		//Engine steps
		//Action step
		void actionStep(ObjectLst* all_objects);
		void globalAction(SDL_Event* event);

		//Draw step
		void drawStep(ObjectLst* all_objects);
		ObjectLst* drawSort(ObjectLst* all_objects);

		//Collision step
		void collisionStep(ObjectLst* all_objects);

		//Physics step
		void physicsStep(ObjectLst* all_objects);

		//Cleaning up threads
		void threadCleanup();

		//Object list building/destruction
		ObjectLst* buildFullObjLst();
		void freeFullObjLst(ObjectLst* all_objects);

		//Saving logic
		void saveGame();
		void loadGame();

		//Handles the default collision between objects
		void handleDefaultCollision(Object* obj1, Hitbox* box1, Object* obj2, Hitbox* box2);

		//Zones
		ZoneLst* zones;
		ZoneLst* active_zones;

		ThreadLst* threads;

		//Render
		Camera* camera;
		SDL_Window* window;
		SDL_Renderer* renderer;

		//State tracking
		GAME_STATE state;

		//A frame counter so we can have timed events trigger every X frames
		unsigned long long timer;
};

#include "../Factory/Factory.hpp"
#include "../Zone/ZoneFactory/ZoneFactory.hpp"

#endif