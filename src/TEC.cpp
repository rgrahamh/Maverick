#include "./TEC.hpp"

int main(int argc, char** argv){
	engine = new Engine(zones);
	engine->start();
	delete engine;
	return 0;
}
