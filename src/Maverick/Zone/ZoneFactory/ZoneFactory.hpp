#ifndef ZONEFACTORY_H
#define ZONEFACTORY_H

#include "./ZoneIncludes.hpp"
#include "../../../led/Zones/led/led.hpp"
#include "../../../Cyberena/Zones/TestZone/TestZone.hpp"
#include "../../../Cyberena/Zones/global/global.hpp"

void loadZone(const char* zone_name, Engine* engine, ObjectList* add_objects);

#endif