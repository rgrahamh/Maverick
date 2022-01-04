#include "./Factory.hpp"

Character* buildCharacter(const char* name, float start_x, float start_y, float friction, float mass, RACE race, STYLE style, Stats* stats,
                          Mastery* mastery, Abilities* abilities, CONTROL_TYPE control, Equipment* equipment, InvSlot** inventory){
    Character* player = new Character(name, start_x, start_y, friction, mass, race, style, stats, mastery, abilities, control, equipment, inventory, 1);

    //Neutral position
    player->addAnimation("neutral_up");
    player->addSprite("neutral_up", "./assets/sprites/old_game_resources/Up_Neutral.bmp", 256, 0, 0);
    player->addAnimation("neutral_down");
    player->addSprite("neutral_down", "./assets/sprites/old_game_resources/Down_Neutral.bmp", 256, 0, 0);
    player->addAnimation("neutral_left");
    player->addSprite("neutral_left", "./assets/sprites/old_game_resources/Left_Neutral.bmp", 256, 0, 0);
    player->addAnimation("neutral_right");
    player->addSprite("neutral_right", "./assets/sprites/old_game_resources/Right_Neutral.bmp", 256, 0, 0);

    //Walking position
    player->addAnimation("walk_up");
    player->addSprite("walk_up", "./assets/sprites/old_game_resources/Up_Left_Foot.bmp", 256, 0, 0);
    player->addSprite("walk_up", "./assets/sprites/old_game_resources/Up_Neutral.bmp", 256, 0, 0);
    player->addSprite("walk_up", "./assets/sprites/old_game_resources/Up_Right_Foot.bmp", 256, 0, 0);

    player->addAnimation("walk_down");
    player->addSprite("walk_down", "./assets/sprites/old_game_resources/Down_Left_Foot.bmp", 256, 0, 0);
    player->addSprite("walk_down", "./assets/sprites/old_game_resources/Down_Neutral.bmp", 256, 0, 0);
    player->addSprite("walk_down", "./assets/sprites/old_game_resources/Down_Right_Foot.bmp", 256, 0, 0);
    

    player->addAnimation("walk_left");
    player->addSprite("walk_left", "./assets/sprites/old_game_resources/Left_Left_Foot.bmp", 256, 0, 0);
    player->addSprite("walk_left", "./assets/sprites/old_game_resources/Left_Neutral.bmp", 256, 0, 0);
    player->addSprite("walk_left", "./assets/sprites/old_game_resources/Left_Right_Foot.bmp", 256, 0, 0);

    player->addAnimation("walk_right");
    player->addSprite("walk_right", "./assets/sprites/old_game_resources/Right_Left_Foot.bmp", 256, 0, 0);
    player->addSprite("walk_right", "./assets/sprites/old_game_resources/Right_Neutral.bmp", 256, 0, 0);
    player->addSprite("walk_right", "./assets/sprites/old_game_resources/Right_Right_Foot.bmp", 256, 0, 0);

    //Adding the walking hitbox
    player->addHitbox(ELLIPSE, 87, 165, 45, 21, COLLISION | HURTBOX);
    //player->addHitbox(0, 7, RECT, 0, 165, 174, 42, COLLISION | HURTBOX);

    //Setting the starting animation
    player->setAnimation("neutral_down");

    //Set player scale
    player->setScale(0.4, 0.4);

    return player;
}

Object* buildPillar(const char* name, float start_x, float start_y){
    Object* pillar = new Object(name, start_x, start_y, 1, 1000, 1);

    pillar->addAnimation("default");
    pillar->addSprite("default", "./assets/sprites/environment/pillar.bmp", 0, 0, 0);
    pillar->setAnimation("default");
    pillar->addHitbox("default", ELLIPSE, 16, 120, 16, 8, COLLISION | ENVIRONMENT);
    //pillar->addHitbox(0, RECT, 0, 112, 32, 16, COLLISION | ENVIRONMENT);

    pillar->setScale(2.0, 2.0);

    return pillar;
}

Object* buildBackground(const char* name, float start_x, float start_y){
    Object* background = new Object(name, start_x, start_y, 1, 1, 0);

    background->addAnimation("default");
    background->addSprite("default", "./assets/sprites/old_game_resources/Floor_Tile.bmp", 0, 0, 0);
    background->setAnimation("default");

    background->setScale(2.5, 2.5);

    return background;
}