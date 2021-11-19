#include "led.hpp"
extern Engine* engine;

void buildEditor(){
    SDL_Window* window = engine->getWindow();

    Zone* led = new Zone("led");

    UITextBox* zone_name = new UITextBox("zone_name", 0, 0, 0.1, 0.075, 1, 1, window, "./assets/fonts/luximr.ttf", "Zone Placeholder", 0, 24, ALIGNMENT::CENTER, ALIGNMENT::CENTER, "./assets/sprites/ui/basic_border");
    zone_name->addSprite(0, "./assets/sprites/ui/blue_box_small.png");
    led->addUIElement(zone_name);

    UITextBox* object_list = new UITextBox("object_list", 0, 0.075, 0.1, 0.85, 1, 1, window, "./assets/fonts/luximr.ttf", "Objects in zone:", 0, 20, ALIGNMENT::NORMAL, ALIGNMENT::NORMAL, "./assets/sprites/ui/basic_border", BORDER_TYPE::ALL_BORDERS, 14);
    object_list->addSprite(0, "./assets/sprites/ui/blue_box_small.png");
    led->addUIElement(object_list);

    UITextBox* layer_display = new UITextBox("draw_layer_display", 0, 0.925, 0.1, 0.075, 1, 1, window, "./assets/fonts/luximr.ttf", "All draw layers", 0.0, 12, ALIGNMENT::CENTER, ALIGNMENT::CENTER, "./assets/sprites/ui/basic_border");
    layer_display->addSprite(0, "./assets/sprites/ui/blue_box_small.png");
    led->addUIElement(layer_display);

    for(int i = 0; i < 10; i++){
        //HOTBAR GENERATION; NEEDS SOME SORT OF FRAME FOR DISPLAYING OBJECTS
    }

    UITextBox* status_bar = new UITextBox("status_bar", 0.1, 0.97, 0.7, 0.03, 1, 1, window, "./assets/fonts/luximr.ttf", "Ready", 0.0, 12);
    status_bar->addSprite(0, "./assets/sprites/ui/black.png");
    led->addUIElement(status_bar);

    UITextBox* tool_selection = new UITextBox("tool_select", 0.8, 0.97, 0.2, 0.03, 1, 1, window, "./assets/fonts/luximr.ttf", "Select", 0.0, 12);
    tool_selection->addSprite(0, "./assets/sprites/ui/black.png");
    led->addUIElement(tool_selection);

    UITextBox* object_attr = new UITextBox("object_attr", 0.8, 0.2, 0.2, 0.77, 1, 1, window, "./assets/fonts/luximr.ttf", "Selected object attributes", 0.0, 20, ALIGNMENT::NORMAL, ALIGNMENT::NORMAL, "./assets/sprites/ui/basic_border", BORDER_TYPE::ALL_BORDERS, 14);
    object_attr->addSprite(0, "./assets/sprites/ui/blue_box_small.png");
    led->addUIElement(object_attr);

    engine->addZone(led);
    engine->activateZone("led");
}