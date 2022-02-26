#include "FilterTest.hpp"
#include "../../../Maverick/Engine/Engine.hpp"
#include "../../../Maverick/Filter/ColorFilter/ColorFilter.hpp"

extern Engine* engine;
void buildFilterTest(){
    Zone* zone = new Zone("FilterTest", 0, 0);

    Object* base_img = new Object("pokemon_town", 0, 0, 0, 0, 1, 0);
    base_img->addAnimation("default", 1);
    base_img->addFrame("default");
    base_img->addSpriteSet("default", "default");
    base_img->addSprite("default", "default", "./assets/channel-mixing-test/original/pokemon-emerald.bmp");
    base_img->setAnimation("default");
    zone->addObject(base_img);

    ColorFilter* filter = new ColorFilter(0, 0,
                                          "./assets/channel-mixing-test/add-masks/pokemon-emerald-red-add.bmp",
                                          "./assets/channel-mixing-test/remove-masks/pokemon-emerald-blue-remove.bmp");
    
    zone->addFilter(filter);

    engine->addZone(zone);
    engine->activateZone("FilterTest");
}