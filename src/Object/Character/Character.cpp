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

/** Calculates any actions taken; should be overridden by children if used
 * @param event The event being interpreted
 */
void Character::action(sf::Event event){}

void Character::process(){
    //Checking to see if we're still sliding
    if(this->sliding == true){
        if(this->xV + this->yV < 1){
            this->sliding = false;
        }
    }
}

/** Called during the draw step
 * @param window The window that content is being drawn to
 */
void Character::draw(){}

/** Called on object collision; should be overridden by children if you want collision logic.
 * @param other The other object
 * @param this_hitbox The hitbox that collided from this object
 * @param other_hitbox The hitbox that collided from the other object
 */
void Character::onCollide(Object* other, Hitbox* this_hitbox, Hitbox* other_hitbox){
	unsigned int other_type = other_hitbox->getType();
	unsigned int this_type = this_hitbox->getType();

	if(this_type & COLLISION){
		if(other_type & COLLISION && other_type & MOVABLE){
			float x_diff = this->x - this->old_x;
			float y_diff = this->y - this->old_y;

			other->setX(other->getX() + x_diff);
			other->setY(other->getY() + y_diff);
		}
		else if(other_type & COLLISION){
			//Make a rollback class?
			float new_x = this->x;
			float new_y = this->y;

			this->y = this->old_y;

			if(this_hitbox->checkCollision(other_hitbox)){
				this->y = new_y;
				this->x = this->old_x;
				if(this_hitbox->checkCollision(other_hitbox)){
					this->y = this->old_y;
					this->x = new_x;
					if(this_hitbox->checkCollision(other_hitbox)){
						this->y = this->old_y;
					}
				}
			}
		}
	}
}