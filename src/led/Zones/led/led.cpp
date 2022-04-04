#include "led.hpp"
extern Engine* engine;

void buildEditor(){
    SDL_Window* window = engine->getWindow();

    Zone* led = new Zone("led", 0, 0);
    engine->addZone(led);

    UITextBox* zone_name = new UITextBox("zone_name", 0, 0, 0.1, 0.075, 1, "./assets/fonts/BitPotionExt.ttf", "Zone Placeholder", 0, 16, ALIGNMENT::CENTER_ALIGN, ALIGNMENT::CENTER_ALIGN, "./assets/sprites/ui/basic_border");
    zone_name->addAnimation("background", 1);
    zone_name->addFrame("background", 0);
    zone_name->addSprite("background", "default", "./assets/sprites/ui/blue_box_small.bmp");
    zone_name->setAnimation("background");
    led->addUIElement(zone_name);

    UITextBox* object_list = new UITextBox("object_list", 0, 0.075, 0.1, 0.85, 1, "./assets/fonts/BitPotionExt.ttf", "Objects in zone:", 0, 24, ALIGNMENT::STANDARD_ALIGN, ALIGNMENT::STANDARD_ALIGN, "./assets/sprites/ui/basic_border", BORDER_TYPE::ALL_BORDERS, 4);
    object_list->addAnimation("background", 1);
    object_list->addFrame("background", 0);
    object_list->addSprite("background", "default", "./assets/sprites/ui/blue_box_small.bmp");
    object_list->setAnimation("background");
    led->addUIElement(object_list);

    UITextBox* layer_display = new UITextBox("draw_layer_display", 0, 0.925, 0.1, 0.075, 1, "./assets/fonts/BitPotionExt.ttf", "All draw layers", 0.0, 24, ALIGNMENT::CENTER_ALIGN, ALIGNMENT::CENTER_ALIGN, "./assets/sprites/ui/basic_border");
    layer_display->addAnimation("background", 1);
    layer_display->addFrame("background", 0);
    layer_display->addSprite("background", "default", "./assets/sprites/ui/blue_box_small.bmp");
    layer_display->setAnimation("background");
    led->addUIElement(layer_display);

    for(int i = 0; i < 10; i++){
        char hotbar_name[8];
        strcpy(hotbar_name, "hotbar");
        sprintf(hotbar_name+6, "%i", i);
        hotbar_name[7] = 0;
        UIObjectFrame* hotbar = new UIObjectFrame(hotbar_name, 0.1 + (i * 0.07), 0.925, 0.07, 0.045, 1, nullptr, "./assets/sprites/ui/basic_border", BORDER_TYPE::ALL_BORDERS, 24);
        hotbar->addAnimation("background", 1);
        hotbar->addFrame("background", 0);
        hotbar->addSprite("background", "default", "./assets/sprites/ui/black.bmp");
        hotbar->setAnimation("background");
        led->addUIElement(hotbar);
    }

    UITextBox* status_bar = new UITextBox("status_bar", 0.1, 0.97, 0.7, 0.03, 1, "./assets/fonts/BitPotionExt.ttf", "Ready", 0.0, 24);
    status_bar->addAnimation("background", 1);
    status_bar->addFrame("background", 0);
    status_bar->addSprite("background", "default", "./assets/sprites/ui/black.bmp");
    status_bar->setAnimation("background");
    led->addUIElement(status_bar);

    UITextBox* tool_selection = new UITextBox("tool_select", 0.8, 0.97, 0.2, 0.03, 1, "./assets/fonts/BitPotionExt.ttf", "Select", 0.0, 24);
    tool_selection->addAnimation("background", 1);
    tool_selection->addFrame("background", 0);
    tool_selection->addSprite("background", "default", "./assets/sprites/ui/black.bmp");
    tool_selection->setAnimation("background");
    led->addUIElement(tool_selection);

    UITextBox* object_attr = new UITextBox("object_attr", 0.8, 0.2, 0.2, 0.77, 1, "./assets/fonts/BitPotionExt.ttf", "Selected object attributes", 0.0, 24, ALIGNMENT::STANDARD_ALIGN, ALIGNMENT::STANDARD_ALIGN, "./assets/sprites/ui/basic_border", BORDER_TYPE::ALL_BORDERS, 4);
    object_attr->addAnimation("background", 1);
    object_attr->addFrame("background", 0);
    object_attr->addSprite("background", "default", "./assets/sprites/ui/blue_box_small.bmp");
    object_attr->setAnimation("background");
    led->addUIElement(object_attr);

    UILevelPort* ui_level_port = new UILevelPort("level_port", 0.1, 0, 0.7, 0.925);
    led->addUIElement(ui_level_port);

    //CameraReference* cam_ref = new CameraReference("cam_ref", 0.0, 0.0);
    //led->addObject(cam_ref);

    //Add a level area section

    //engine->getCamera()->setReference(cam_ref);

    //REMOVE THIS SECTION EVENTUALLY; led should eventually be the only thing loaded
    /*ZoneList* zones = engine->getActiveZones();
    while(zones != nullptr){
        ObjectList* objects = zones->zone->getObjects();
        while(objects != nullptr){
            objects->obj->setActive(false);
            objects = objects->next;
        }
        zones = zones->next;
    }*/

    engine->activateZone("led");
}