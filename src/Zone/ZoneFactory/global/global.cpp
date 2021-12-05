#include "global.hpp"
extern Engine* engine;

void buildGlobal(){
    SDL_Window* window = engine->getWindow();

    Zone* global = new Zone("global");

    //Create the player
    Character* player = buildCharacter("player", 0.0f, 0.0f, 0.75, 185.0, HUMAN, ATTACKER, new Stats(), new Mastery(), new Abilities(), CONTROL_TYPE::KEYBOARD, new Equipment(), NULL);
    global->addObject(player);

    //Create the pause menu
    UIElement* pause_menu = new UIElement("pause_menu", 0, 0, 1, 1, 1, 0, UI_OBJECT_TYPE::WINDOW, window);
    pause_menu->setActive(false);
    pause_menu->addSprite(0, "./assets/sprites/ui/shade.png", 0, 0, 0);

    //Create the pause text
    UIText* pause_text = new UIText("pause_text", 0.0, 0.0, 1.0, 1.0, 1, 1, window, "./assets/fonts/luximr.ttf", "Paused", 0.0, 24, ALIGNMENT::CENTER, ALIGNMENT::CENTER);
    pause_text->setColor(255, 255, 255);
    pause_menu->addElement(pause_text);

    pause_menu->setVisible(false);
    pause_menu->setActive(false);

    global->addUIElement(pause_menu);

    //UITextBox* text_box = new UITextBox("text_box", 0.2, 0.8, 0.6, 0.15, 1, 1, window, "./assets/fonts/luximr.ttf", "This is a text box! Here's some sample text. I think this is great and will be pretty long.");
    //text_box->setColor(255, 255, 255);
    //text_box->addSprite(0, "./assets/sprites/ui/black.png", 0, 0, 0, 0.6, 0.15);

    engine->addZone(global);
    engine->activateZone("global");

    engine->getCamera()->setReference(player);
}