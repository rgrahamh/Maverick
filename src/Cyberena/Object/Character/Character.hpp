#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdlib.h>

#include "../Object.hpp"
#include "../Item/Item.hpp"
#include "../Weapon/Weapon.hpp"
#include "../Armor/Armor.hpp"
#include "Cyberena/Ability/Ability.hpp"
#include "Race/Race.hpp"
#include "Style/Style.hpp"
#include "Maverick/Control/Control.hpp"

#define WALK_SPEED 1.5
#define JUMP_SPEED 3.0
#define KICK_SPEED 6.0

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

enum CHARACTER_STATE{
	NEUTRAL_UP    = 0,
	NEUTRAL_DOWN  = 1,
	NEUTRAL_LEFT  = 2,
	NEUTRAL_RIGHT = 3,
	WALK_UP       = 4,
	WALK_DOWN     = 5,
	WALK_LEFT     = 6,
	WALK_RIGHT    = 7
};

enum CONTROL_TYPE{
	NONE = 0,
	KEYBOARD = 1,
	GAMEPAD = 2,
	AI = 4,
	NETWORK = 8
};

struct CharacterControl{
	double x_movement = 0;
	double y_movement = 0;
	bool jump = false;
};

class Character : public Object, public Race, public Style{
	public:
		Character(const char* name, float start_x, float start_y, float start_z, float friction, float mass, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, CONTROL_TYPE control, Equipment* equipment = NULL, InvSlot** inventory = NULL, int draw_layer = 1);
		virtual ~Character();

		//Useful for actions on other objects & input
		virtual void action(Control* control);
		virtual void process(uint64_t delta, unsigned int steps);

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
		CharacterControl character_control;

		void clearCharacterControl();
};

#endif
