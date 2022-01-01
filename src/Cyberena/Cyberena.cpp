#include "../Maverick/Engine/Engine.hpp"
extern Engine* engine;

int main(int argc, char** argv){
	engine = new Engine();
	//engine->addZone(loadZone)
	engine->start();
	delete engine;
	return 0;
}