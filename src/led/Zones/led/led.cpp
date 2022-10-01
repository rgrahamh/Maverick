#include "led.hpp"

//Maverick includes
#include "Maverick/Zone/Zone.hpp"
#include "Maverick/Engine/Engine.hpp"
#include "Maverick/UIElement/UITextBox/UITextBox.hpp"
#include "Maverick/UIElement/UIObjectFrame/UIObjectFrame.hpp"
#include "Maverick/UIElement/UITabbedWindow/UITabbedWindow.hpp"
#include "Maverick/Object/CameraReference/CameraReference.hpp"

//led includes
#include "led/UIElement/UILevelPort/UILevelPort.hpp"

#include "stdio.h"

void buildEditor(){
    Engine* engine = Engine::getInstance();

    Zone* led = new Zone("led", 0, 0);
    engine->addZone(led);

    UIElement* level_group = new UIElement("level_group", 0.0, 0.0, 1.0, 1.0, 0);

    engine->addSurface("blue_box", SDL_LoadBMP("./assets/sprites/ui/blue_box_small.bmp"));
    engine->addSurface("black_box", SDL_LoadBMP("./assets/sprites/ui/black.bmp"));

    engine->addSurface("basic_border_top", SDL_LoadBMP("./assets/sprites/ui/basic_border_top.bmp"));
    engine->addSurface("basic_border_bottom", SDL_LoadBMP("./assets/sprites/ui/basic_border_bottom.bmp"));
    engine->addSurface("basic_border_left", SDL_LoadBMP("./assets/sprites/ui/basic_border_left.bmp"));
    engine->addSurface("basic_border_right", SDL_LoadBMP("./assets/sprites/ui/basic_border_right.bmp"));

    UITextBox* zone_name = new UITextBox("zone_name", 0, 0, 0.1, 0.075, 1, "mavwhite", "Zone Placeholder", 0, 1, ALIGNMENT::CENTER_ALIGN, ALIGNMENT::CENTER_ALIGN);
    Animation* zone_name_anim = new Animation("zone_name_anim", 1);
    zone_name_anim->addFrame(0);
    zone_name_anim->addSpriteSet("background");
    zone_name_anim->addSprite("background", "blue_box", 0, 0);
    zone_name->setBackground(zone_name_anim);
    level_group->addSubelement(zone_name);

    UITextBox* object_list = new UITextBox("object_list", 0, 0.075, 0.1, 0.85, 1, "mavwhite", "Objects in zone:", 0, 1, ALIGNMENT::STANDARD_ALIGN, ALIGNMENT::STANDARD_ALIGN);
    Animation* object_list_anim = new Animation("object_name_anim", 1);
    object_list_anim->addFrame(0);
    object_list_anim->addSpriteSet("background");
    object_list_anim->addSprite("background", "blue_box", 0, 0);
    object_list->setBackground(object_list_anim);
    level_group->addSubelement(object_list);

    UITextBox* layer_display = new UITextBox("draw_layer_display", 0, 0.925, 0.1, 0.075, 1, "mavwhite", "All draw layers", 5.0, 1, ALIGNMENT::CENTER_ALIGN, ALIGNMENT::CENTER_ALIGN);
    Animation* blue_box = new Animation("layer_display_anim", 1);
    blue_box->addFrame(0);
    blue_box->addSprite("background", "blue_box", 0, 0);
    layer_display->setBackground(blue_box);
    level_group->addSubelement(layer_display);

    Animation* black_box = new Animation("status_anim", 1);
    black_box->addFrame(0);
    black_box->addSprite("background", "black_box", 0, 0);
    for(int i = 0; i < 10; i++){
        char hotbar_name[8];
        strcpy(hotbar_name, "hotbar");
        sprintf(hotbar_name+6, "%i", i);
        hotbar_name[7] = 0;
        UIObjectFrame* hotbar = new UIObjectFrame(hotbar_name, 0.1 + (i * 0.07), 0.925, 0.07, 0.045, 1, nullptr);
        hotbar->setBackground(black_box);
        level_group->addSubelement(hotbar);
    }

    UITextBox* status_bar = new UITextBox("status_bar", 0.1, 0.97, 0.7, 0.03, 1, "mavwhite", "Ready", 0.0, 1);
    status_bar->setBackground(black_box);
    level_group->addSubelement(status_bar);

    UITextBox* tool_selection = new UITextBox("tool_select", 0.8, 0.97, 0.2, 0.03, 1, "mavwhite", "Select", 0.0, 1);
    tool_selection->setBackground(black_box);
    level_group->addSubelement(tool_selection);

    UITextBox* object_attr = new UITextBox("object_attr", 0.8, 0.2, 0.2, 0.77, 1, "mavwhite", "Selected object attributes", 0.0, 1, ALIGNMENT::STANDARD_ALIGN, ALIGNMENT::STANDARD_ALIGN);
    object_attr->setBackground(blue_box);
    level_group->addSubelement(object_attr);

    UILevelPort* ui_level_port = new UILevelPort("level_port", 0.1, 0, 0.7, 0.925);
    level_group->addSubelement(ui_level_port);

    led->addUIElement(level_group);
    level_group->setActive(true);
    level_group->setVisible(true);

    //Object Ediror
    /*UITabbedWindow* tabbed_window = new UITabbedWindow("selector_pane", 0.0, 0.0, 0.25, 0.9, 1, 0.05, "mavwhite", 1);
    tabbed_window->setBackground(black_box);

    UITextBox* first_tab = new UITextBox("first", 0.0, 0.05, 0.25, 0.85, 1, "mavwhite", "first", 0, 1, ALIGNMENT::CENTER_ALIGN, ALIGNMENT::STANDARD_ALIGN);
    first_tab->setBackground(blue_box);
    tabbed_window->addSubelement(first_tab);

    UITextBox* second_tab = new UITextBox("second", 0.0, 0.05, 0.25, 0.85, 1, "mavwhite", "second", 0, 1, ALIGNMENT::CENTER_ALIGN, ALIGNMENT::STANDARD_ALIGN);
    second_tab->setBackground(blue_box);
    tabbed_window->addSubelement(second_tab);

    led->addUIElement(tabbed_window);*/

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