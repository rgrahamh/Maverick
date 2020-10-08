#include "./ZoneFactory.hpp"

void loadZone(const char* zone_name, Engine* engine, ObjectLst* add_objects){
	if(strcmp(zone_name, "Test Zone") == 0){
		buildTestZone(engine, add_objects);
	}
}