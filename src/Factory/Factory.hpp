#ifndef FACTORY_H
#define FACTORY_H

#include "../Object/Character/Player/Player.hpp"
#include "../Animation/Hitbox/HitRect/HitRect.hpp"
#include "../Animation/Hitbox/HitEllipse/HitCone/HitCone.hpp"
#include "../Animation/Hitbox/HitEllipse/HitEllipse.hpp"

Player* buildPlayer(float start_x, float start_y, float friction, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, Equipment* equipment, InvSlot** inventory, unsigned int draw_layer);
Object* buildPillar(float start_x, float start_y, int draw_layer);

#endif