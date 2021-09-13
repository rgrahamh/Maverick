#include "./Factory.hpp"


Character* buildCharacter(char* name, float start_x, float start_y, float friction, float mass, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, CONTROL_TYPE control, Equipment* equipment, InvSlot** inventory){
    Character* player = new Character("player", start_x, start_y, friction, mass, race, style, stats, mastery, abilities, control, equipment, inventory, 12);

    //Neutral position
    player->addSprite(0, "./assets/sprites/old_game_resources/Up_Neutral.png", 256, 0, 0);
    player->addSprite(1, "./assets/sprites/old_game_resources/Down_Neutral.png", 256, 0, 0);
    player->addSprite(2, "./assets/sprites/old_game_resources/Left_Neutral.png", 256, 0, 0);
    player->addSprite(3, "./assets/sprites/old_game_resources/Right_Neutral.png", 256, 0, 0);

    //Walking position
    player->addSprite(4, "./assets/sprites/old_game_resources/Up_Left_Foot.png", 256, 0, 0);
    player->addSprite(4, "./assets/sprites/old_game_resources/Up_Neutral.png", 256, 0, 0);
    player->addSprite(4, "./assets/sprites/old_game_resources/Up_Right_Foot.png", 256, 0, 0);
    player->addSprite(5, "./assets/sprites/old_game_resources/Down_Left_Foot.png", 256, 0, 0);
    player->addSprite(5, "./assets/sprites/old_game_resources/Down_Neutral.png", 256, 0, 0);
    player->addSprite(5, "./assets/sprites/old_game_resources/Down_Right_Foot.png", 256, 0, 0);
    player->addSprite(6, "./assets/sprites/old_game_resources/Left_Left_Foot.png", 256, 0, 0);
    player->addSprite(6, "./assets/sprites/old_game_resources/Left_Neutral.png", 256, 0, 0);
    player->addSprite(6, "./assets/sprites/old_game_resources/Left_Right_Foot.png", 256, 0, 0);
    player->addSprite(7, "./assets/sprites/old_game_resources/Right_Left_Foot.png", 256, 0, 0);
    player->addSprite(7, "./assets/sprites/old_game_resources/Right_Neutral.png", 256, 0, 0);
    player->addSprite(7, "./assets/sprites/old_game_resources/Right_Right_Foot.png", 256, 0, 0);

    //Adding the walking hitbox
    player->addHitbox(0, 7, ELLIPSE, 87, 165, 45, 21, COLLISION | HURTBOX);
    //player->addHitbox(0, 7, RECT, 0, 165, 174, 42, COLLISION | HURTBOX);

    //Setting the starting animation
    player->setAnimation(1);

    //Set player scale
    player->setScale(0.4, 0.4);

    return player;
}

Object* buildPillar(char* name, float start_x, float start_y){
    Object* pillar = new Object("pillar", start_x, start_y, 1, 1000, 1);

    pillar->addSprite(0, "./assets/sprites/environment/pillar.png", 0, 0, 0);
    pillar->addHitbox(0, ELLIPSE, 16, 120, 16, 8, COLLISION | ENVIRONMENT);
    //pillar->addHitbox(0, RECT, 0, 112, 32, 16, COLLISION | ENVIRONMENT);

    pillar->setScale(2.0, 2.0);

    return pillar;
}

Object* buildBackground(char* name, float start_x, float start_y){
    Object* background = new Object("background", start_x, start_y, 1, 1, 1, 0);

    background->addSprite(0, "./assets/sprites/old_game_resources/Floor_Tile.png", 0, 0, 0);

    background->setScale(2.5, 2.5);

    return background;
}