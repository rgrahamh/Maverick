#include "./Factory.hpp"

Character* buildCharacter(const char* name, float start_x, float start_y, float friction, float mass, RACE race, STYLE style, Stats* stats,
                          Mastery* mastery, Abilities* abilities, CONTROL_TYPE control, Equipment* equipment, InvSlot** inventory){
    Character* player = new Character(name, start_x, start_y, friction, mass, race, style, stats, mastery, abilities, control, equipment, inventory, 1);

    //Neutral position
    player->addAnimation("neutral_up", 1);
    player->addAnimationFrame("neutral_up", 0);
    player->addHitbox("neutral_up", ELLIPSE, 87, 165, 45, 21, COLLISION | HURTBOX, 0);
    player->addSprite("neutral_up", "day", "./assets/sprites/old_game_resources/Up_Neutral.bmp");

    player->addAnimation("neutral_down", 1);
    player->addAnimationFrame("neutral_down", 0);
    player->addHitbox("neutral_down", ELLIPSE, 87, 165, 45, 21, COLLISION | HURTBOX, 0);
    player->addSprite("neutral_down", "day", "./assets/sprites/old_game_resources/Down_Neutral.bmp");

    player->addAnimation("neutral_left", 1);
    player->addAnimationFrame("neutral_left", 0);
    player->addHitbox("neutral_left", ELLIPSE, 87, 165, 45, 21, COLLISION | HURTBOX, 0);
    player->addSprite("neutral_left", "day", "./assets/sprites/old_game_resources/Left_Neutral.bmp");

    player->addAnimation("neutral_right", 1);
    player->addAnimationFrame("neutral_right", 0);
    player->addHitbox("neutral_right", ELLIPSE, 87, 165, 45, 21, COLLISION | HURTBOX, 0);
    player->addSprite("neutral_right", "day", "./assets/sprites/old_game_resources/Right_Neutral.bmp");

    //Walking position
    player->addAnimation("walk_up", 1);
    player->addAnimationFrame("walk_up", 256, 3);
    player->addSprite("walk_up", "day", "./assets/sprites/old_game_resources/Up_Left_Foot.bmp");
    player->addHitbox("walk_up", ELLIPSE, 87, 165, 45, 21, COLLISION | HURTBOX, 0, 0);
    player->addSprite("walk_up", "day", "./assets/sprites/old_game_resources/Up_Neutral.bmp");
    player->addHitbox("walk_up", ELLIPSE, 87, 165, 45, 21, COLLISION | HURTBOX, 1, 0);
    player->addSprite("walk_up", "day", "./assets/sprites/old_game_resources/Up_Right_Foot.bmp");
    player->addHitbox("walk_up", ELLIPSE, 87, 165, 45, 21, COLLISION | HURTBOX, 2, 0);

    player->addAnimation("walk_down", 1);
    player->addAnimationFrame("walk_down", 256, 3);
    player->addSprite("walk_down", "day", "./assets/sprites/old_game_resources/Down_Left_Foot.bmp");
    player->addHitbox("walk_down", ELLIPSE, 87, 165, 45, 21, COLLISION | HURTBOX, 0, 0);
    player->addSprite("walk_down", "day", "./assets/sprites/old_game_resources/Down_Neutral.bmp");
    player->addHitbox("walk_down", ELLIPSE, 87, 165, 45, 21, COLLISION | HURTBOX, 1, 0);
    player->addSprite("walk_down", "day", "./assets/sprites/old_game_resources/Down_Right_Foot.bmp");
    player->addHitbox("walk_down", ELLIPSE, 87, 165, 45, 21, COLLISION | HURTBOX, 2, 0);
    
    player->addAnimation("walk_left", 1);
    player->addAnimationFrame("walk_left", 256, 3);
    player->addSprite("walk_left", "day", "./assets/sprites/old_game_resources/Left_Left_Foot.bmp");
    player->addHitbox("walk_left", ELLIPSE, 87, 165, 45, 21, COLLISION | HURTBOX, 0, 0);
    player->addSprite("walk_left", "day", "./assets/sprites/old_game_resources/Left_Neutral.bmp");
    player->addHitbox("walk_left", ELLIPSE, 87, 165, 45, 21, COLLISION | HURTBOX, 1, 0);
    player->addSprite("walk_left", "day", "./assets/sprites/old_game_resources/Left_Right_Foot.bmp");
    player->addHitbox("walk_left", ELLIPSE, 87, 165, 45, 21, COLLISION | HURTBOX, 2, 0);

    player->addAnimation("walk_right", 1);
    player->addAnimationFrame("walk_right", 256, 3);
    player->addSprite("walk_right", "day", "./assets/sprites/old_game_resources/Right_Left_Foot.bmp");
    player->addHitbox("walk_right", ELLIPSE, 87, 165, 45, 21, COLLISION | HURTBOX, 0, 0);
    player->addSprite("walk_right", "day", "./assets/sprites/old_game_resources/Right_Neutral.bmp");
    player->addHitbox("walk_right", ELLIPSE, 87, 165, 45, 21, COLLISION | HURTBOX, 1, 0);
    player->addSprite("walk_right", "day", "./assets/sprites/old_game_resources/Right_Right_Foot.bmp");
    player->addHitbox("walk_right", ELLIPSE, 87, 165, 45, 21, COLLISION | HURTBOX, 2, 0);

    //Setting the starting animation
    player->setAnimation("neutral_down");

    //Set player scale
    player->setScale(0.4, 0.4);

    return player;
}

Object* buildPillar(const char* name, float start_x, float start_y){
    Object* pillar = new Object(name, start_x, start_y, 1, 1000, 1);

    pillar->addAnimation("default", 1);
    pillar->addAnimationFrame("default");
    pillar->addSprite("default", "day", "./assets/sprites/environment/pillar.bmp");
    pillar->setAnimation("default");
    pillar->addHitbox("default", ELLIPSE, 16, 120, 16, 8, COLLISION | ENVIRONMENT, 0, 1);
    //pillar->addHitbox(0, RECT, 0, 112, 32, 16, COLLISION | ENVIRONMENT);

    pillar->setScale(2.0, 2.0);

    return pillar;
}

Object* buildBackground(const char* name, float start_x, float start_y){
    Object* background = new Object(name, start_x, start_y, 1, 1, 0);

    background->addAnimation("default", 1);
    background->addAnimationFrame("default", 0);
    background->addSprite("default", "day", "./assets/sprites/old_game_resources/Floor_Tile.bmp");
    background->setAnimation("default");

    background->setScale(2.5, 2.5);

    return background;
}