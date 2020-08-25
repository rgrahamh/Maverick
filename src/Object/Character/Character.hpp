#ifndef CHARACTER_H
#define CHARACTER_H

#include "../Item/Item.hpp"
#include "../Item/Weapon/Weapon.hpp"
#include "../Item/Armor/Armor.hpp"

typedef struct InventorySlot{
	ITEM_TYPE item_type;
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
	Helmet* helmet;
	Suit* suit;
	Guards* guards;
	Gloves* gloves;
	Pants* pants;
} Equipment;

class Character{
	public:
		Character(Stats* stats, Inventory* inventory, Mastery* mastery, Equipment* equipment, Abilities* abilities);
		~Character();

	protected:
		bool overdrive;
		Stats* stats;
		BattleStats* battle_stats;
		InvSlot* inventory[128];
		Mastery* mastery;
		Equipment* equipment;
		Abilities* abilities;
}

#endif
