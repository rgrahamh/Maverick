#include "led.hpp"
extern Engine* engine;

void buildEditor(){
    SDL_Window* window = engine->getWindow();

    Zone* led = new Zone("led", 0, 0);

    UITextBox* zone_name = new UITextBox("zone_name", 0, 0, 0.1, 0.075, window, 1, "./assets/fonts/luximr.ttf", "Zone Placeholder", 0, 24, ALIGNMENT::CENTER, ALIGNMENT::CENTER, "./assets/sprites/ui/basic_border");
    zone_name->addAnimation("background");
    zone_name->addSprite("background", "./assets/sprites/ui/blue_box_small.bmp");
    zone_name->setAnimation("background");
    led->addUIElement(zone_name);

    UITextBox* object_list = new UITextBox("object_list", 0, 0.075, 0.1, 0.85, window, 1, "./assets/fonts/luximr.ttf", "Objects in zone:", 0, 20, ALIGNMENT::NORMAL, ALIGNMENT::NORMAL, "./assets/sprites/ui/basic_border", BORDER_TYPE::ALL_BORDERS, 14);
    object_list->addAnimation("background");
    object_list->addSprite("background", "./assets/sprites/ui/blue_box_small.bmp");
    object_list->setAnimation("background");
    led->addUIElement(object_list);

    UITextBox* layer_display = new UITextBox("draw_layer_display", 0, 0.925, 0.1, 0.075, window, 1, "./assets/fonts/luximr.ttf", "All draw layers", 0.0, 12, ALIGNMENT::CENTER, ALIGNMENT::CENTER, "./assets/sprites/ui/basic_border");
    layer_display->addAnimation("background");
    layer_display->addSprite("background", "./assets/sprites/ui/blue_box_small.bmp");
    layer_display->setAnimation("background");
    led->addUIElement(layer_display);

    for(int i = 0; i < 10; i++){
        char hotbar_name[8];
        strcpy(hotbar_name, "hotbar");
        sprintf(hotbar_name+6, "%i", i);
        hotbar_name[7] = 0;
        UIObjectFrame* hotbar = new UIObjectFrame(hotbar_name, 0.1 + (i * 0.07), 0.925, 0.07, 0.045, window, 1, nullptr, "./assets/sprites/ui/basic_border", BORDER_TYPE::ALL_BORDERS, 14);
        hotbar->addSprite("background", "./assets/sprites/ui/black.bmp");
        hotbar->setAnimation("background");
        led->addUIElement(hotbar);
    }

    UITextBox* status_bar = new UITextBox("status_bar", 0.1, 0.97, 0.7, 0.03, window, 1, "./assets/fonts/luximr.ttf", "Ready", 0.0, 12);
    status_bar->addAnimation("background");
    status_bar->addSprite("background", "./assets/sprites/ui/black.bmp");
    status_bar->setAnimation("background");
    led->addUIElement(status_bar);

    UITextBox* tool_selection = new UITextBox("tool_select", 0.8, 0.97, 0.2, 0.03, window, 1, "./assets/fonts/luximr.ttf", "Select", 0.0, 12);
    tool_selection->addAnimation("background");
    tool_selection->addSprite("background", "./assets/sprites/ui/black.bmp");
    tool_selection->setAnimation("background");
    led->addUIElement(tool_selection);

    UITextBox* object_attr = new UITextBox("object_attr", 0.8, 0.2, 0.2, 0.77, window, 1, "./assets/fonts/luximr.ttf", "Selected object attributes", 0.0, 20, ALIGNMENT::NORMAL, ALIGNMENT::NORMAL, "./assets/sprites/ui/basic_border", BORDER_TYPE::ALL_BORDERS, 14);
    object_attr->addAnimation("background");
    object_attr->addSprite("background", "./assets/sprites/ui/blue_box_small.bmp");
    object_attr->setAnimation("background");
    led->addUIElement(object_attr);

    UILevelPort* ui_level_port = new UILevelPort("level_port", 0.1, 0, 0.7, 0.925, window);
    led->addUIElement(ui_level_port);

    CameraReference* cam_ref = new CameraReference("cam_ref", 0.0, 0.0);
    led->addObject(cam_ref);

    //Add a level area section

    engine->addZone(led);
    engine->activateZone("led");

    engine->getCamera()->setReference(cam_ref);
}