#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdlib.h>

#include "../../Item/Item.hpp"
#include "../../Weapon/Weapon.hpp"
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

class Character : public Object, public Race, public Style{
	public:
		Character(float start_x, float start_y, float friction, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, Equipment* equipment, InvSlot** inventory, unsigned int draw_layer, unsigned int animation_num);
		virtual ~Character();
		//Useful for actions on other objects & input
		virtual void action(sf::Event event);
		virtual void process();
		virtual void draw();

	protected:
		bool overdrive;
		Stats* stats;
		InvSlot** inventory;
		Mastery* mastery;
		Equipment* equipment;
		Abilities* abilities;
};

#endif
