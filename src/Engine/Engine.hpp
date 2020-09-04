#ifndef ENGINE_H
#define ENGINE_H

#include "../Zone/Zone.hpp"
#include "../Camera/Camera.hpp"

#include "../Object/Character/Character.hpp"
#include "../Object/Character/Player/Player.hpp"

enum GAME_STATE{
	TITLE,
	OVERWORLD,
	BATTLE,
	DISCUSSION,
	PAUSE
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
		void inputStep();
		void drawStep();
		void collisionStep();
		void physicsStep();

		void saveGame();
		void loadGame();

		void addZone(Zone* zone);
		void activateZone(char* zone_name);
		void deactivateZone(char* zone_name);

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