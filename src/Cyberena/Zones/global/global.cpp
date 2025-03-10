#include "global.hpp"
#include "Maverick/Engine/Engine.hpp"
#include "Maverick/UIElement/UITextBox/UITextBox.hpp"
#include "Cyberena/Object/Character/Character.hpp"
#include "Maverick/FileHandler/Loader/Loader.hpp"

void buildGlobal(){
    Zone* global = new Zone("global", 0, 0);
    Engine* engine = Engine::getInstance();
    engine->addZone(global);

    FILE* file = fopen("./assets/fonts/mavwhite.fnt", "rb");
    if(file == nullptr){
        printf("Can't find the Maverick font file!\n");
        return;
    }
    uint8_t resource_type;
    fread(&resource_type, 1, 1, file);
    loadFont(file, "mavwhite");

    Sound* new_sound = new Sound;
    new_sound->name = "step";
    new_sound->sample = Mix_LoadWAV("./assets/audio/effects/step.wav");
	engine->addSound("step", new_sound);

    engine->addSurface("black_box", SDL_LoadBMP("./assets/sprites/ui/black.bmp"));
    engine->addSurface("shade", SDL_LoadBMP("./assets/sprites/ui/shade.bmp"));

    //Create the player
    Character* player = buildCharacter("player", 0.0f, 0.0f, 0.0f, 0.975, 185.0, HUMAN, ATTACKER, new Stats(), new Mastery(), new Abilities(), CONTROL_TYPE::KEYBOARD, new Equipment(), NULL);
    global->addObject(player);
    engine->getCamera()->setReference(player);

    //Create the pause menu
    UIElement* pause_menu = new UIElement("pause_menu", 0, 0, 1, 1, 0);
    pause_menu->setActive(false);
    Animation* pause_animation = new Animation("pause_animation", 1);
    pause_animation->addFrame(0);
    pause_animation->addSpriteSet("default");
    pause_animation->addSprite("default", "black_box", 0, 0);
    pause_menu->setBackground(pause_animation);

    //Create the pause text
    UIText* pause_text = new UIText("pause_text", 0.0, 0.0, 1.0, 1.0, 1, "mavwhite", "Paused", 0.0, 1, ALIGNMENT::CENTER_ALIGN, ALIGNMENT::CENTER_ALIGN);
    pause_text->setColor(255, 255, 255);
    pause_menu->addSubelement(pause_text);

    pause_menu->setVisible(false);
    pause_menu->setActive(false);

    global->addUIElement(pause_menu);

    //UITextBox* text_box = new UITextBox("text_box", 0.2, 0.8, 0.6, 0.15, 1, 1, "./assets/fonts/nokiafc22.ttf", "This is a text box! Here's some sample text. I think this is great and will be pretty long.");
    //text_box->setColor(255, 255, 255);
    //text_box->addSprite(0, "black_box", 0, 0, 0, 0.6, 0.15);

    engine->activateZone("global");

}