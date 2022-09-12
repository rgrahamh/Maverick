#include "./LoadingZone.hpp"
#include "Maverick/Engine/Engine.hpp"
#include "Maverick/Zones/ZoneFactory/ZoneFactory.hpp"

LoadingZone::LoadingZone(const char* name, const char* zone_to_load, float start_x, float start_y, float start_z, double width, double height, double depth, uint32_t matching_type, int layer)
           : Object(name, start_x, start_y, start_z, 0, 0, 0){
    if(this->addAnimation("loading", 1) == -1){
        printf("Could not add LoadingZone animation\n");
    }
    if(this->addFrame("loading") == -1){
        printf("Could not frame to LoadingZone animation\n");
    }
    if(this->addHitbox("loading", HITBOX_SHAPE::HIT_RECT, 0, 0, 0, width, height, depth, HITBOX_TYPE::TRIGGER, 0)){
        printf("Could not add hitbox to the frame\n");
    }
    this->setAnimation("loading");

    this->matching_type = matching_type;
    this->zone_to_load = StrDeepCopy(zone_to_load);
}

void LoadingZone::onCollide(Object* other, Hitbox* this_hitbox, Hitbox* other_hitbox){
    if(other->getType() == this->matching_type && Engine::getInstance()->getZone(this->zone_to_load) == nullptr){
        printf("Loading zone: %s\n", this->zone_to_load);
        loadZone(this->zone_to_load);
    }
}