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
#include "../../Control/Control.hpp"

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
	UP_NEUTRAL    = 0,
	DOWN_NEUTRAL  = 1,
	LEFT_NEUTRAL  = 2,
	RIGHT_NEUTRAL = 3,
	UP_WALK       = 4,
	DOWN_WALK     = 5,
	LEFT_WALK     = 6,
	RIGHT_WALK    = 7
};

enum CONTROL_TYPE{
	KEYBOARD = 1,
	GAMEPAD = 2,
	AI = 4,
	NETWORK = 8,
	NONE = 16
};

class Character : public Object, public Race, public Style{
	public:
		Character(const char* name, float start_x, float start_y, float friction, float mass, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, CONTROL_TYPE control, Equipment* equipment = NULL, InvSlot** inventory = NULL, unsigned int animation_num = 12, int draw_layer = 1);
		virtual ~Character();

		//Useful for actions on other objects & input
		virtual void action(Control* control);
		virtual void process(uint32_t delta);

		virtual void onCollide(Object* other, Hitbox* this_hitbox, Hitbox* other_hitbox);

		bool isStanding();
		bool isWalking();

	protected:
		bool overdrive;
		bool sliding;
		uint8_t control;
		uint8_t control_num;
		Stats* stats;
		InvSlot** inventory;
		Mastery* mastery;
		Equipment* equipment;
		Abilities* abilities;
};

#endif
