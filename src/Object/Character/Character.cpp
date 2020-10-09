#include "./Character.hpp"

Character::Character(float start_x, float start_y, float friction, float mass, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, CONTROL_TYPE control, Equipment* equipment, InvSlot** inventory, unsigned animation_num)
	: Object(start_x, start_y, friction, mass, animation_num),
	Race(race),
	Style(style){
	//Setting the struct pointers; these are the non-optional params
	this->stats = stats;
	this->mastery = mastery;
	this->abilities = abilities;
	this->control = control;

	//Initializing inventory
	if(inventory != NULL){
		this->inventory = inventory;
	}else{
		this->inventory = (InvSlot**)calloc(sizeof(InvSlot), 128);
	}

	//Initializing equipment
	if(equipment != NULL){
		this->equipment = equipment;
	}else{
		this->equipment = new Equipment();
	}

	//Initializing keys
    keys.up_arrow = 0;
    keys.down_arrow = 0;
    keys.left_arrow = 0;
    keys.right_arrow = 0;

	this->overdrive=false;
}

Character::~Character(){
	free(this->inventory);
	delete this->equipment;
}

/** Calculates any actions taken; should be overridden by children if used
 * @param event The event being interpreted
 */
void Character::action(sf::Event event){
    if(event.type == sf::Event::KeyPressed){
        if(event.key.code == sf::Keyboard::Up){
            this->keys.up_arrow = 1;
            //Both on or both off
            if(!(this->keys.left_arrow ^ this->keys.right_arrow)){
                this->setAnimation(UP_WALK);
            }
        }
        else if(event.key.code == sf::Keyboard::Down){
            this->keys.down_arrow = 1;
            //Both on or both off
            if(!(this->keys.left_arrow ^ this->keys.right_arrow)){
                this->setAnimation(DOWN_WALK);
            }
        }
        else if(event.key.code == sf::Keyboard::Left){
            this->keys.left_arrow = 1;
            if(keys.right_arrow && !(keys.up_arrow || keys.down_arrow)){
                this->setAnimation(RIGHT_NEUTRAL);
            }
            else if (keys.right_arrow && keys.down_arrow){
                this->setAnimation(DOWN_WALK);
            }
            else if (keys.right_arrow && keys.up_arrow){
                this->setAnimation(UP_WALK);
            }
            else{
                this->setAnimation(LEFT_WALK);
            }
        }
        else if(event.key.code == sf::Keyboard::Right){
            this->keys.right_arrow = 1;
            if(keys.left_arrow && !(keys.up_arrow || keys.down_arrow)){
                this->setAnimation(LEFT_NEUTRAL);
            }
            else if (keys.left_arrow && keys.down_arrow){
                this->setAnimation(DOWN_WALK);
            }
            else if (keys.left_arrow && keys.up_arrow){
                this->setAnimation(UP_WALK);
            }
            else{
                this->setAnimation(RIGHT_WALK);
            }
        }
    }

    else if(event.type == sf::Event::KeyReleased){
        if(event.key.code == sf::Keyboard::Up){
            this->keys.up_arrow = 0;

            //If down and nothing else or down and everything else
            if((this->keys.down_arrow && !(this->keys.left_arrow || this->keys.right_arrow)) || (this->keys.up_arrow && this->keys.right_arrow && this->keys.left_arrow)){
                this->setAnimation(DOWN_WALK);
            }
            //If left and right
            else if(this->keys.left_arrow && this->keys.right_arrow){
                this->setAnimation(UP_NEUTRAL);
            }
            //If left
            else if(this->keys.left_arrow){
                this->setAnimation(LEFT_WALK);
            }
            //If right
            else if(this->keys.right_arrow){
                this->setAnimation(RIGHT_WALK);
            }
            //If nothing else
            else{
                this->setAnimation(UP_NEUTRAL);
            }
        }

        else if(event.key.code == sf::Keyboard::Down){
            this->keys.down_arrow = 0;

            //If up and nothing else or up and everything else
            if((this->keys.up_arrow && !(this->keys.left_arrow || this->keys.right_arrow)) || (this->keys.up_arrow && this->keys.right_arrow && this->keys.left_arrow)){
                this->setAnimation(UP_WALK);
            }
            //If left and right
            else if(this->keys.left_arrow && this->keys.right_arrow){
                this->setAnimation(DOWN_NEUTRAL);
            }
            //If left
            else if(this->keys.left_arrow){
                this->setAnimation(LEFT_WALK);
            }
            //If right
            else if(this->keys.right_arrow){
                this->setAnimation(RIGHT_WALK);
            }
            //If nothing else
            else{
                this->setAnimation(DOWN_NEUTRAL);
            }
        }

        else if(event.key.code == sf::Keyboard::Left){
            this->keys.left_arrow = 0;

            //If right and nothing else or right and everything else
            if((this->keys.right_arrow && !(this->keys.up_arrow && this->keys.down_arrow)) || (this->keys.right_arrow && this->keys.up_arrow && this->keys.down_arrow)){
                this->setAnimation(RIGHT_WALK);
            }
            //If up and down
            else if(this->keys.up_arrow && this->keys.down_arrow){
                this->setAnimation(LEFT_NEUTRAL);
            }
            //If up
            else if(this->keys.up_arrow){
                this->setAnimation(UP_WALK);
            }
            //If down
            else if(this->keys.down_arrow){
                this->setAnimation(DOWN_WALK);
            }
            //If nothing else
            else{
                this->setAnimation(LEFT_NEUTRAL);
            }
        }

        else if(event.key.code == sf::Keyboard::Right){
            this->keys.right_arrow = 0;

            //If left and nothing else or left and everything else
            if((this->keys.left_arrow && !(this->keys.up_arrow && this->keys.down_arrow)) || (this->keys.left_arrow && this->keys.up_arrow && this->keys.down_arrow)){
                this->setAnimation(LEFT_WALK);
            }
            //If up and down
            else if(this->keys.up_arrow && this->keys.down_arrow){
                this->setAnimation(RIGHT_NEUTRAL);
            }
            //If up
            else if(this->keys.up_arrow){
                this->setAnimation(UP_WALK);
            }
            //If down
            else if(this->keys.down_arrow){
                this->setAnimation(DOWN_WALK);
            }
            //If nothing else
            else{
                this->setAnimation(RIGHT_NEUTRAL);
            }
        }
    }
}

void Character::process(){
    //Checking to see if we're still sliding
    if(this->sliding == true){
        if(this->xV + this->yV < 1){
            this->sliding = false;
        }
    }

    //If we're not sliding (in an actionable state)
    if(!this->sliding){
		if(this->control == CONTROL_TYPE::KEYBOARD){
			if(this->keys.up_arrow){
				this->yA -= 2;
			}
			if(this->keys.down_arrow){
				this->yA += 2;
			}
			if(this->keys.left_arrow){
				this->xA -= 2;
			}
			if(this->keys.right_arrow){
				this->xA += 2;
			}
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