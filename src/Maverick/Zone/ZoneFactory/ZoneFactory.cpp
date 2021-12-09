#include "./ZoneFactory.hpp"

#include "../../../Cyberena/Zones/TestZone/TestZone.hpp"
#include "../../../Cyberena/Zones/global/global.hpp"
#include "../../../led/Zones/led/led.hpp"

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
}