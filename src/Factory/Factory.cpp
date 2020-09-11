#include "./Factory.hpp"


Player* buildPlayer(float start_x, float start_y, float friction, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, Equipment* equipment, InvSlot** inventory, unsigned int draw_layer, unsigned int animation_num){
    Player* player = new Player(0.0f, 0.0f, 0.75, HUMAN, ATTACKER, new Stats(), new Mastery(), new Abilities(), new Equipment(), NULL, 1, 12);

    //Neutral position
    player->addSprite(0, "./assets/sprites/old_game_resources/Up_Neutral.png", 16, 0, 0);
    player->addSprite(1, "./assets/sprites/old_game_resources/Down_Neutral.png", 16, 0, 0);
    player->addSprite(2, "./assets/sprites/old_game_resources/Left_Neutral.png", 16, 0, 0);
    player->addSprite(3, "./assets/sprites/old_game_resources/Right_Neutral.png", 16, 0, 0);

    //Walking position
    player->addSprite(4, "./assets/sprites/old_game_resources/Up_Left_Foot.png", 16, 0, 0);
    player->addSprite(4, "./assets/sprites/old_game_resources/Up_Neutral.png", 16, 0, 0);
    player->addSprite(4, "./assets/sprites/old_game_resources/Up_Right_Foot.png", 16, 0, 0);
    player->addSprite(5, "./assets/sprites/old_game_resources/Down_Left_Foot.png", 16, 0, 0);
    player->addSprite(5, "./assets/sprites/old_game_resources/Down_Neutral.png", 16, 0, 0);
    player->addSprite(5, "./assets/sprites/old_game_resources/Down_Right_Foot.png", 16, 0, 0);
    player->addSprite(6, "./assets/sprites/old_game_resources/Left_Left_Foot.png", 16, 0, 0);
    player->addSprite(6, "./assets/sprites/old_game_resources/Left_Neutral.png", 16, 0, 0);
    player->addSprite(6, "./assets/sprites/old_game_resources/Left_Right_Foot.png", 16, 0, 0);
    player->addSprite(7, "./assets/sprites/old_game_resources/Right_Left_Foot.png", 16, 0, 0);
    player->addSprite(7, "./assets/sprites/old_game_resources/Right_Neutral.png", 16, 0, 0);
    player->addSprite(7, "./assets/sprites/old_game_resources/Right_Right_Foot.png", 16, 0, 0);

    player->setAnimation(1);

    //Set player scale
    player->setScale(0.8, 0.8);

    return player;
}