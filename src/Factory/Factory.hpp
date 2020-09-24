#ifndef FACTORY_H
#define FACTORY_H

#include "../Object/Character/Player/Player.hpp"
#include "../Animation/Hitbox/HitRect/HitRect.hpp"
#include "../Animation/Hitbox/HitEllipse/HitCone/HitCone.hpp"
#include "../Animation/Hitbox/HitEllipse/HitEllipse.hpp"

Player* buildPlayer(float start_x, float start_y, float friction, float mass, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, Equipment* equipment, InvSlot** inventory);
Object* buildPillar(float start_x, float start_y);

#endif