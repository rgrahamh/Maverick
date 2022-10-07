#define SDL_MAIN_HANDLED
#include "Maverick/Engine/Engine.hpp"

int main(int argc, char** argv){
	Engine* engine = Engine::getInstance();
	//engine->addZone(loadZone)
	engine->start();
	return 0;
}