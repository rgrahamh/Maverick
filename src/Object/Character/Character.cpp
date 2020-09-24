#include "./Character.hpp"

Character::Character(float start_x, float start_y, float friction, float mass, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, Equipment* equipment, InvSlot** inventory, unsigned animation_num)
	: Object(start_x, start_y, friction, mass, animation_num),
	Race(race),
	Style(style){
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
		this->equipment = new Equipment();
	}

	this->overdrive=false;
}

Character::~Character(){
	free(this->inventory);
	delete this->equipment;
}

void Character::action(sf::Event event){}

void Character::process(){
    //Checking to see if we're still sliding
    if(this->sliding == true){
        if(this->xV + this->yV < 1){
            this->sliding = false;
        }
    }
}

void Character::draw(){}

void Character::onCollide(Object* other, Hitbox* this_hitbox, Hitbox* other_hitbox){
	printf("Collided!\n");
}