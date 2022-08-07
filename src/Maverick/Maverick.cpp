#include "Engine/Engine.cpp"

int main(int argc, char** argv){
	Engine* engine = Engine::getInstance();
	engine->start();
	return 0;
}
