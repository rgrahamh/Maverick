#ifndef FACTORY_H
#define FACTORY_H

#include "../Entity/Object/Character/Character.hpp"

Character* buildCharacter(const char* name, float start_x, float start_y, float start_z, float friction, float mass, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, CONTROL_TYPE control, Equipment* equipment, InvSlot** inventory);
Object* buildPillar(const char* name, float start_x, float start_y);
Object* buildBackground(const char* name, float start_x, float start_y);

#endif