#include "./ZoneFactory.hpp"

#include "../../../Cyberena/Zones/TestZone/TestZone.hpp"
#include "../../../Cyberena/Zones/global/global.hpp"
#include "../../../led/Zones/led/led.hpp"

void loadZone(const char* zone_name, Engine* engine, ObjectList* add_objects){
	if(strcmp(zone_name, "Test Zone") == 0){
		buildTestZone(engine, add_objects);
	}
	else if(strcmp(zone_name, "led") == 0){
		buildEditor();
	}
	else if(strcmp(zone_name, "global") == 0){
		buildGlobal();
	}
}