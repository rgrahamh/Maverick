#ifndef ENGINE_H
#define ENGINE_H

#include "../Zone/Zone.hpp"
#include "../Camera/Camera.hpp"

#include "../Object/Character/Character.hpp"
#include "../Object/Character/Player/Player.hpp"

#include "../Factory/Factory.hpp"

enum GAME_STATE{
	TITLE,
	OVERWORLD,
	BATTLE,
	DISCUSSION,
	PAUSE,
	EXIT
};

class Engine{
	public:
		Engine();
		~Engine();

		//Game engine run
		void start();

	private:
		void gameLoop();

		//Engine steps
		void actionStep(ObjectLst* all_objects);
		void drawStep(ObjectLst* all_objects);
		void collisionStep(ObjectLst* all_objects);
		void physicsStep(ObjectLst* all_objects);

		//Object list building/destruction
		ObjectLst* buildFullObjLst();
		void freeFullObjLst(ObjectLst* all_objects);

		//Saving logic
		void saveGame();
		void loadGame();

		//Zone addition/state handling
		void addZone(Zone* zone);
		void activateZone(const char* zone_name);
		void deactivateZone(const char* zone_name);

		//Loading a zone from file
		Zone* loadZone(unsigned char zone_num);

		//Zones
		ZoneLst* zones;
		ZoneLst* active_zones;

		//Render
		Camera* camera;
		sf::RenderWindow* window;

		//State tracking
		GAME_STATE state;


};

#endif