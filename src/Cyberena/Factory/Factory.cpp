#include "./Factory.hpp"

Character* buildCharacter(const char* name, float start_x, float start_y, float start_z, float friction, float mass, RACE race, STYLE style, Stats* stats,
                          Mastery* mastery, Abilities* abilities, CONTROL_TYPE control, Equipment* equipment, InvSlot** inventory){
    Character* player = new Character(name, start_x, start_y, start_z, friction, mass, race, style, stats, mastery, abilities, control, equipment, inventory, 1);

    //Neutral position
    player->addAnimation("neutral_up", 1);
    player->addFrame("neutral_up", 0);
    player->addSprite("neutral_up", "day", "./assets/sprites/old_game_resources/Up_Neutral.bmp", 0, 0, 34, 44);
    player->addHitbox("neutral_up", HIT_ELLIPSE, 17, 40, 0, 12, 4, 44, COLLISION | HURTBOX, 0);
    player->addHitbox("neutral_up", HIT_ELLIPSE, 17, 40, -540, 12, 4, 540, GROUNDING_ZONE, 0);

    player->addAnimation("neutral_down", 1);
    player->addFrame("neutral_down", 0);
    player->addSprite("neutral_down", "day", "./assets/sprites/old_game_resources/Down_Neutral.bmp", 0, 0, 34, 44);
    player->addHitbox("neutral_down", HIT_ELLIPSE, 17, 40, 0, 12, 4, 44, COLLISION | HURTBOX, 0);
    player->addHitbox("neutral_down", HIT_ELLIPSE, 17, 40, -540, 12, 4, 540, GROUNDING_ZONE, 0);

    player->addAnimation("neutral_left", 1);
    player->addFrame("neutral_left", 0);
    player->addSprite("neutral_left", "day", "./assets/sprites/old_game_resources/Left_Neutral.bmp", 0, 0, 34, 44);
    player->addHitbox("neutral_left", HIT_ELLIPSE, 17, 40, 0, 12, 4, 44, COLLISION | HURTBOX, 0);
    player->addHitbox("neutral_left", HIT_ELLIPSE, 17, 40, -540, 12, 4, 540, GROUNDING_ZONE, 0);

    player->addAnimation("neutral_right", 1);
    player->addFrame("neutral_right", 0);
    player->addSprite("neutral_right", "day", "./assets/sprites/old_game_resources/Right_Neutral.bmp", 0, 0, 34, 44);
    player->addHitbox("neutral_right", HIT_ELLIPSE, 17, 40, 0, 12, 4, 44, COLLISION | HURTBOX, 0);
    player->addHitbox("neutral_right", HIT_ELLIPSE, 17, 40, -540, 12, 4, 540, GROUNDING_ZONE, 0);

    //Walking position
    player->addAnimation("walk_up", 1);
    player->addFrame("walk_up", 256, 3);
    player->addSprite("walk_up", "day", "./assets/sprites/old_game_resources/Up_Left_Foot.bmp", 0, 0, 34, 44);
    player->addSprite("walk_up", "day", "./assets/sprites/old_game_resources/Up_Neutral.bmp", 0, 0, 34, 44);
    player->addSprite("walk_up", "day", "./assets/sprites/old_game_resources/Up_Right_Foot.bmp", 0, 0, 34, 44);

    player->addAnimation("walk_down", 1);
    player->addFrame("walk_down", 256, 3);
    player->addSprite("walk_down", "day", "./assets/sprites/old_game_resources/Down_Left_Foot.bmp", 0, 0, 34, 44);
    player->addSprite("walk_down", "day", "./assets/sprites/old_game_resources/Down_Neutral.bmp", 0, 0, 34, 44);
    player->addSprite("walk_down", "day", "./assets/sprites/old_game_resources/Down_Right_Foot.bmp", 0, 0, 34, 44);
    
    player->addAnimation("walk_left", 1);
    player->addFrame("walk_left", 256, 3);
    player->addSprite("walk_left", "day", "./assets/sprites/old_game_resources/Left_Left_Foot.bmp", 0, 0, 34, 44);
    player->addSprite("walk_left", "day", "./assets/sprites/old_game_resources/Left_Neutral.bmp", 0, 0, 34, 44);
    player->addSprite("walk_left", "day", "./assets/sprites/old_game_resources/Left_Right_Foot.bmp", 0, 0, 34, 44);

    player->addAnimation("walk_right", 1);
    player->addFrame("walk_right", 256, 3);
    player->addSprite("walk_right", "day", "./assets/sprites/old_game_resources/Right_Left_Foot.bmp", 0, 0, 34, 44);
    player->addSprite("walk_right", "day", "./assets/sprites/old_game_resources/Right_Neutral.bmp", 0, 0, 34, 44);
    player->addSprite("walk_right", "day", "./assets/sprites/old_game_resources/Right_Right_Foot.bmp", 0, 0, 34, 44);

    for(int i = 0; i < 3; i++){
        player->addHitbox("walk_right", HIT_ELLIPSE, 17, 40, 0, 12, 4, 44, COLLISION | HURTBOX, i);
        player->addHitbox("walk_left", HIT_ELLIPSE, 17, 40, 0, 12, 4, 44, COLLISION | HURTBOX, i);
        player->addHitbox("walk_down", HIT_ELLIPSE, 17, 40, 0, 12, 4, 44, COLLISION | HURTBOX, i);
        player->addHitbox("walk_up", HIT_ELLIPSE, 17, 40, 0, 12, 4, 44, COLLISION | HURTBOX, i);

        //Ground hitboxes
        player->addHitbox("walk_right", HIT_ELLIPSE, 17, 40, -540, 12, 4, 540, GROUNDING_ZONE, i);
        player->addHitbox("walk_left", HIT_ELLIPSE, 17, 40, -540, 12, 4, 540, GROUNDING_ZONE, i);
        player->addHitbox("walk_down", HIT_ELLIPSE, 17, 40, -540, 12, 4, 540, GROUNDING_ZONE, i);
        player->addHitbox("walk_up", HIT_ELLIPSE, 17, 40, -540, 12, 4, 540, GROUNDING_ZONE, i);
    }

    //Setting the starting animation
    player->setAnimation("neutral_down");

    return player;
}

Object* buildPillar(const char* name, float start_x, float start_y){
    Object* pillar = new Object(name, start_x, start_y, 0, 1, 1000, 0);

    pillar->addAnimation("default", 1);
    pillar->addFrame("default");
    pillar->addSprite("default", "day", "./assets/sprites/environment/pillar.bmp");
    pillar->setAnimation("default");
    pillar->addHitbox("default", HIT_ELLIPSE, 16, 120, 0, 18, 8, 120, COLLISION | ENVIRONMENT, 0);

    return pillar;
}

Object* buildCrate(const char* name, float start_x, float start_y){
    Object* crate = new Object(name, start_x, start_y, 0, 1, 500, 0);

    crate->addAnimation("default", 1);
    crate->addFrame("default");
    crate->addSprite("default", "day", "./assets/sprites/environment/CrateTest.bmp");
    crate->setAnimation("default");
    crate->addHitbox("default", HIT_RECT, 0, 16, 0, 48, 48, 16, HITBOX_TYPE::COLLISION | HITBOX_TYPE::ENVIRONMENT, 0);
    crate->setDrawAxis(17);

    return crate;
}

Object* buildBackground(const char* name, float start_x, float start_y){
    Object* background = new Object(name, start_x, start_y, 0, 1, 1, 0, 0, 0);

    background->addAnimation("default", 1);
    background->addFrame("default", 0);
    background->addSprite("default", "day", "./assets/sprites/old_game_resources/Floor_Tile.bmp");
    background->setAnimation("default");

    return background;
}

GlassBall* buildGlassBloodBall(const char* name, float start_x, float start_y){
    GlassBall* glassBall = new GlassBall(name, start_x, start_y, 0, 0.15, 5, 20);

    glassBall->addAnimation("neutral", 1);
    glassBall->addFrame("neutral", 0);
    glassBall->addSprite("neutral", "day", "./assets/sprites/test_assets/GlassBloodBall/GlassBloodBallNeutral.bmp");
    glassBall->addHitbox("neutral", HIT_ELLIPSE, 3, 3, 0, 4, 4, 3, HITBOX_TYPE::COLLISION | HITBOX_TYPE::MOVABLE, 0);

    glassBall->addAnimation("rolling", 1);
    for(int i = 0; i < 7; i++){
        glassBall->addFrame("rolling", 100);
        glassBall->addHitbox("rolling", HIT_ELLIPSE, 3, 3, 0, 4, 4, 3, HITBOX_TYPE::COLLISION | HITBOX_TYPE::MOVABLE, i);
    }
    glassBall->addSprite("rolling", "day", "./assets/sprites/test_assets/GlassBloodBall/GlassBloodBallRolling1.bmp");
    glassBall->addSprite("rolling", "day", "./assets/sprites/test_assets/GlassBloodBall/GlassBloodBallRolling2.bmp");
    glassBall->addSprite("rolling", "day", "./assets/sprites/test_assets/GlassBloodBall/GlassBloodBallRolling3.bmp");
    glassBall->addSprite("rolling", "day", "./assets/sprites/test_assets/GlassBloodBall/GlassBloodBallRolling4.bmp");
    glassBall->addSprite("rolling", "day", "./assets/sprites/test_assets/GlassBloodBall/GlassBloodBallRolling5.bmp");
    glassBall->addSprite("rolling", "day", "./assets/sprites/test_assets/GlassBloodBall/GlassBloodBallRolling6.bmp");
    glassBall->addSprite("rolling", "day", "./assets/sprites/test_assets/GlassBloodBall/GlassBloodBallRolling7.bmp");

    return glassBall;
}