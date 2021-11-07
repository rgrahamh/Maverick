#include "./ZoneFactory.hpp"

void loadZone(const char* zone_name, Engine* engine, ObjectList* add_objects){
	if(strcmp(zone_name, "Test Zone") == 0){
		buildTestZone(engine, add_objects);
	}
	else if(strcmp(zone_name, "led") == 0){
		buildEditor();
	}
}