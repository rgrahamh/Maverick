#include "./ZoneFactory.hpp"
#include "../../../Maverick/Engine/Engine.hpp"

//This function will eventually open a file w/ the passed in <zone_name>.pld
void loadZone(const char* zone_name){
	if(strcmp(zone_name, "Test Zone") == 0){
		buildTestZone();
	}
	else if(strcmp(zone_name, "global") == 0){
		buildGlobal();
	}

	Engine::getInstance()->cleanupThread(std::this_thread::get_id());
}