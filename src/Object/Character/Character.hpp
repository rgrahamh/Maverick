#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdlib.h>

#include "../../Item/Item.hpp"
#include "../../Object/Weapon/Weapon.hpp"
#include "../../Armor/Armor.hpp"
#include "../../Ability/Ability.hpp"
#include "../Object.hpp"
#include "./Race/Race.hpp"
#include "./Style/Style.hpp"

typedef struct InventorySlot{
	Item* item;
	unsigned int amnt;
} InvSlot;

typedef struct MasteryList{
	char axe;
	char blaster;
	char claws;
	char knuckles;
	char lance;
	char sword;
} Mastery;

typedef struct AbilitiesList{
	Ability* ability;
	Ability* next;
} Abilities;

//The current stat modifications (proportional; initialized to 1)
typedef struct StatModifications{
	float powerMod;
	int strength;
	float defenseMod;
	float resistanceMod;
} StatMods;

//The base stats list
typedef struct StatsList{
	int health;
	int maxHealth;
	int strength;
	float power;
	float maxPower;
	int defense;
	int resistance;
	int speed;
} Stats;

//The battle stats list (modified by weapon and armor attributes; calc at start of every battle)
typedef struct BattleStatsList{
	int health;
	int strength;
	float shield;
	float power;
	int defense;
	int resistance;
	int speed;
} BattleStats;

//The equipment list (what the character is currently wearing)
typedef struct EquipmentList{
	Item* item;
	//Helmet* helmet;
	//Suit* suit;
	//Guards* guards;
	//Gloves* gloves;
	//Pants* pants;
} Equipment;

enum CHARACTER_ANIMATION{
	UP_NEUTRAL,
	DOWN_NEUTRAL,
	LEFT_NEUTRAL,
	RIGHT_NEUTRAL,
	UP_WALK,
	DOWN_WALK,
	LEFT_WALK,
	RIGHT_WALK
};

enum CONTROL_TYPE{
	KEYBOARD,
	AI,
	NETWORK,
	WANDERING,
	STATIC
};

class Character : public Object, public Race, public Style{
	public:
		Character(float start_x, float start_y, float friction, float mass, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, CONTROL_TYPE control, Equipment* equipment = NULL, InvSlot** inventory = NULL, unsigned int animation_num = 12);
		virtual ~Character();
		//Useful for actions on other objects & input
		virtual void action(SDL_Event* event);
		virtual void process();
		virtual void draw();

		virtual void onCollide(Object* other, Hitbox* this_hitbox, Hitbox* other_hitbox);

	protected:
		bool overdrive;
		bool sliding;
		CONTROL_TYPE control;
		Stats* stats;
		InvSlot** inventory;
		Mastery* mastery;
		Equipment* equipment;
		Abilities* abilities;

        typedef struct KeyboardState{
            bool up_arrow;
            bool down_arrow;
            bool left_arrow;
            bool right_arrow;
        } KeyState;

        KeyState keys;
};

#endif
