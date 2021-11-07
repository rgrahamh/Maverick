#ifndef ZONEFACTORY_H
#define ZONEFACTORY_H

#include "./ZoneIncludes.hpp"
#include "./TestZone/TestZone.hpp"
#include "./led/led.hpp"

void loadZone(const char* zone_name, Engine* engine, ObjectList* add_objects);

#endif