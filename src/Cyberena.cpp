#include "./Cyberena.hpp"

int main(int argc, char** argv){
	engine = new Engine();
	engine->start();
	delete engine;
	return 0;
}
