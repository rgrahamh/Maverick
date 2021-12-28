#include "./TestZone.hpp"
#include "../../../Maverick/Engine/Engine.hpp"
extern Engine* engine;

void buildTestZone(){
    Zone* zone = new Zone("Test Zone", 0, 0);

    //Create pillars
    zone->addObject(buildPillar("pillar1", 800.0, 600.0));
    zone->addObject(buildPillar("pillar2", 900.0, 450.0));
    zone->addObject(buildPillar("pillar3", 700.0, 300.0));
    zone->addObject(buildPillar("pillar4", 750.0, 250.0));
    zone->addObject(buildPillar("pillar5", 300.0, 200.0));

    zone->addObject(buildBackground("background", 0.0, 0.0));

    engine->addZone(zone);
    engine->activateZone(zone->getName());
}