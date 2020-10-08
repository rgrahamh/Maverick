#ifndef ENGINE_H
#define ENGINE_H

#define ROLLBACK_STEP 6.0

#include <thread>

#include "../Zone/Zone.hpp"
#include "../Camera/Camera.hpp"

#include "../Object/Character/Character.hpp"
#include "../Object/Character/Player/Player.hpp"

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
		void actionStep(ObjectLst* all_objects);
		ObjectLst* drawStep(ObjectLst* all_objects);
		void collisionStep(ObjectLst* all_objects);
		void physicsStep(ObjectLst* all_objects);

		//Sorts an object list by draw axis
		ObjectLst* drawSort(ObjectLst* all_objects);

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
		sf::RenderWindow* window;

		//State tracking
		GAME_STATE state;

		//A frame counter so we can have timed events trigger every X frames
		unsigned long long timer;
};

#include "../Factory/Factory.hpp"
#include "../Zone/ZoneFactory/ZoneFactory.hpp"

#endif