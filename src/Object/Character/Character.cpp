#include "./Character.hpp"

Character::Character(Stats* stats, Mastery* mastery, Abilities* abilities, Equipment* equipment = NULL, InvSlot** inventory = NULL){
	//Setting the struct pointers; these are the non-optional params
	this->stats = stats;
	this->mastery = mastery;
	this->abilities = abilities;

	if(inventory != NULL){
		this->inventory = inventory;
	}else{
		this->inventory = (InvSlot**)calloc(sizeof(InvSlot), 128);
	}
	if(equipment != NULL){
		this->equipment = equipment;
	}else{
		this->equipment = (Equipment*)calloc(sizeof(Equipment), 1);
	}

	this->overdrive=false;
}
