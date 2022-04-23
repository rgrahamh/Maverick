#include "Engine/Engine.cpp"
extern Engine* engine;

int main(int argc, char** argv){
	engine = new Engine();
	engine->start();
	delete engine;
	return 0;
}
