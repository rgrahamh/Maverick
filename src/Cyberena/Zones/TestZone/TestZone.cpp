#include "./TestZone.hpp"

void buildTestZone(Engine* engine, ObjectList* add_object){
    Zone* zone = new Zone("Test Zone");

    while(add_object != NULL){
        zone->addObject(add_object->obj);
        add_object = add_object->next;
    }

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