#ifndef FACTORY_H
#define FACTORY_H

#include "../Object/Character/Character.hpp"
#include "../Animation/Hitbox/HitRect/HitRect.hpp"
#include "../Animation/Hitbox/HitEllipse/HitCone/HitCone.hpp"
#include "../Animation/Hitbox/HitEllipse/HitEllipse.hpp"

Character* buildCharacter(char* name, float start_x, float start_y, float friction, float mass, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, CONTROL_TYPE control, Equipment* equipment, InvSlot** inventory);
Object* buildPillar(char* name, float start_x, float start_y);
Object* buildBackground(char* name, float start_x, float start_y);

#endif