#include "./ZoneFactory.hpp"
#include "../../Engine/Engine.hpp"
extern Engine* engine;

//This function will eventually open a file w/ the passed in <zone_name>.pld
void loadZone(const char* zone_name){
	if(strcmp(zone_name, "Test Zone") == 0){
		buildTestZone();
	}
	else if(strcmp(zone_name, "led") == 0){
		buildEditor();
	}
	else if(strcmp(zone_name, "global") == 0){
		buildGlobal();
	}
	else if(strcmp(zone_name, "FilterTest") == 0){
		buildFilterTest();
	}

	engine->cleanupThread(std::this_thread::get_id());
}