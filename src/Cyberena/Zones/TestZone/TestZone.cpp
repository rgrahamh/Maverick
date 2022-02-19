#include "./TestZone.hpp"
#include "../../../Maverick/Engine/Engine.hpp"
extern Engine* engine;

void buildTestZone(){
    Zone* zone = new Zone("Test Zone", 0, 0);
    engine->addZone(zone);

    //Create pillars
    zone->addObject(buildPillar("pillar1", 400.0, 500.0));
    zone->addObject(buildPillar("pillar2", 175.0, 350.0));
    zone->addObject(buildPillar("pillar3", 200.0, 100.0));
    zone->addObject(buildPillar("pillar4", 350.0, 250.0));
    zone->addObject(buildPillar("pillar5", 100.0, 200.0));

    zone->addObject(buildBackground("background", 0.0, 0.0));

    zone->addObject(new LoadingZone("ui_load", "led", 200.0, 100.0, 0.0, 200.0, 200.0, 30.0, EXTENDED_OBJECT_TYPE::CHARACTER));

    engine->activateZone(zone->getName());
}