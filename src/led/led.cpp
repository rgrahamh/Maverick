#define SDL_MAIN_HANDLED
#include "../Maverick/Engine/Engine.hpp"
extern Engine* engine;

int main(int argc, char** argv){
	engine = new Engine();
	engine->start();
	delete engine;
	return 0;
}
