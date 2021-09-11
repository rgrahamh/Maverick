#include "./TestZone.hpp"

void buildTestZone(Engine* engine, ObjectLst* add_object){
    Zone* zone = new Zone("Test Zone");

    while(add_object != NULL){
        zone->addObject(add_object->obj);
        add_object = add_object->next;
    }

    //Create a pillar
    Object* pillar_1 = buildPillar("pillar1", 800.0, 700.0);
    zone->addObject(pillar_1);

    //Create a pillar
    Object* pillar_2 = buildPillar("pillar2", 800.0, 600.0);
    zone->addObject(pillar_2);

    //Create a pillar
    Object* pillar_3 = buildPillar("pillar3", 800.0, 500.0);
    zone->addObject(pillar_3);

    engine->addZone(zone);
    engine->activateZone(zone->getName());
}