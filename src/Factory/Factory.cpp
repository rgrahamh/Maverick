#include "./Factory.hpp"


Player* buildPlayer(float start_x, float start_y, float friction, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, Equipment* equipment, InvSlot** inventory, unsigned int draw_layer){
    Player* player = new Player(start_x, start_y, friction, race, style, stats, mastery, abilities, equipment, inventory, draw_layer, 12);

    //Neutral position
    player->addSprite(0, "./assets/sprites/old_game_resources/Up_Neutral.png", 16, 0, 0);
    player->addHitbox(0, RECT, 0, 93, 186, 93);
    player->addSprite(1, "./assets/sprites/old_game_resources/Down_Neutral.png", 16, 0, 0);
    player->addHitbox(1, RECT, 1, 93, 186, 93);
    player->addSprite(2, "./assets/sprites/old_game_resources/Left_Neutral.png", 16, 0, 0);
    player->addHitbox(2, RECT, 1, 93, 186, 93);
    player->addSprite(3, "./assets/sprites/old_game_resources/Right_Neutral.png", 16, 0, 0);
    player->addHitbox(3, RECT, 1, 93, 186, 93);

    //Walking position
    player->addSprite(4, "./assets/sprites/old_game_resources/Up_Left_Foot.png", 16, 0, 0);
    player->addHitbox(4, RECT, 1, 93, 186, 93);
    player->addSprite(4, "./assets/sprites/old_game_resources/Up_Neutral.png", 16, 0, 0);
    player->addHitbox(4, RECT, 1, 93, 186, 93);
    player->addSprite(4, "./assets/sprites/old_game_resources/Up_Right_Foot.png", 16, 0, 0);
    player->addHitbox(4, RECT, 1, 93, 186, 93);
    player->addSprite(5, "./assets/sprites/old_game_resources/Down_Left_Foot.png", 16, 0, 0);
    player->addHitbox(4, RECT, 1, 93, 186, 93);
    player->addSprite(5, "./assets/sprites/old_game_resources/Down_Neutral.png", 16, 0, 0);
    player->addHitbox(5, RECT, 1, 93, 186, 93);
    player->addSprite(5, "./assets/sprites/old_game_resources/Down_Right_Foot.png", 16, 0, 0);
    player->addHitbox(5, RECT, 1, 93, 186, 93);
    player->addSprite(6, "./assets/sprites/old_game_resources/Left_Left_Foot.png", 16, 0, 0);
    player->addHitbox(5, RECT, 1, 93, 186, 93);
    player->addSprite(6, "./assets/sprites/old_game_resources/Left_Neutral.png", 16, 0, 0);
    player->addHitbox(5, RECT, 1, 93, 186, 93);
    player->addSprite(6, "./assets/sprites/old_game_resources/Left_Right_Foot.png", 16, 0, 0);
    player->addHitbox(6, RECT, 1, 93, 186, 93);
    player->addSprite(7, "./assets/sprites/old_game_resources/Right_Left_Foot.png", 16, 0, 0);
    player->addHitbox(7, RECT, 1, 93, 186, 93);
    player->addSprite(7, "./assets/sprites/old_game_resources/Right_Neutral.png", 16, 0, 0);
    player->addHitbox(7, RECT, 1, 93, 186, 93);
    player->addSprite(7, "./assets/sprites/old_game_resources/Right_Right_Foot.png", 16, 0, 0);
    player->addHitbox(7, RECT, 1, 93, 186, 93);

    player->setAnimation(1);

    //Set player scale
    player->setScale(0.8, 0.8);

    return player;
}

Object* buildPillar(float start_x, float start_y, int draw_layer){
    Object* pillar = new Object(start_x, start_y, 1, 1, 1);

    pillar->addSprite(0, "./assets/sprites/environment/Pillar.png", 0, 0, 0);
    pillar->addHitbox(0, ELLIPSE, 16, 98, 8, 8);


    return pillar;
}