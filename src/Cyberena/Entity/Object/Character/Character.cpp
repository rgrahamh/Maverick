#include "./Character.hpp"
#include "../../../../Maverick/Engine/Engine.hpp"

/** Paramaterized constructor for the Character
 * @param name The character's name
 * @param start_x The character's starting X position
 * @param start_y The character's starting Y position
 * @param start_z The character's starting Z position
 * @param friction The COF for the character
 * @param mass The mass of the character
 * @param race The race of the character
 * @param stats The character's stats struct
 * @param mastry The character's mastery struct
 * @param abilities A pointer to the character's abilities struct
 * @param control The CONTROL_TYPE of what's controlling the character
 * @param equipment The equipment that the character has
 * @param inventory The inventory that the character has
 * @param draw_layer The draw layer of the character
 */
Character::Character(const char* name, float start_x, float start_y, float start_z, float friction, float mass, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, CONTROL_TYPE control, Equipment* equipment, InvSlot** inventory, int draw_layer)
	: Object(name, start_x, start_y, start_z, friction, mass, 11, true, draw_layer),
	Race(race),
	Style(style){
    this->type = EXTENDED_OBJECT_TYPE::CHARACTER;

	//Setting the struct pointers; these are the non-optional params
	this->stats = stats;
	this->mastery = mastery;
	this->abilities = abilities;
	this->control = control;
    this->sliding = false;
	this->control = CONTROL_TYPE::KEYBOARD;
	this->control_num = 0;

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

	this->overdrive=false;
}

/** The character's default destructor
 */
Character::~Character(){
	free(this->inventory);
	delete this->equipment;
}

/** Returns if the character is walking
 * @return If the character is in a neutral animation
 */
bool Character::isStanding(){
    return strncmp("neutral", this->active_animation->getName(), 7) == 0;
}

/** Returns if the character is walking
 * @return If the character is in a walking animation
 */
bool Character::isWalking(){
    return strncmp("walk", this->active_animation->getName(), 4) == 0;
}

/** Clears out the character control struct; run at the start of every action step
 */
void Character::clearCharacterControl(){
	character_control.x_movement = 0;
	character_control.y_movement = 0;
	character_control.jump = false;
}

/** Calculates any actions taken; should be overridden by children if used
 * @param event The event being interpreted
 */
void Character::action(Control* control){

	//Only clear the control if it's a gamepad; otherwise, we'll want to retain these values
	if(this->control == CONTROL_TYPE::GAMEPAD || this->control == CONTROL_TYPE::KEYBOARD){
		this->clearCharacterControl();
	}

	const char* active_animation_name = this->active_animation->getName();

    if(!Engine::getInstance()->checkState(GAME_STATE::PAUSE | GAME_STATE::DISCUSSION | GAME_STATE::TITLE) && !this->sliding){
		//If the character is in an actionable state
		if(this->control == CONTROL_TYPE::KEYBOARD){
			const uint8_t* keys = control->getKeys();
			const uint8_t* digital = control->getDigitalPress();

			//If (pressing no keys or pressing keys in opposite directions or pressing all keys) and was just walking
			if(((!(keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_D]))
			|| (keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_D])
			|| (!keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_D])
			|| (keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_D]))
			&& this->isWalking()){
				if(strcmp("walk_up", active_animation_name) == 0){
					setAnimation("neutral_up");
				}
				else if(strcmp("walk_right", active_animation_name) == 0){
					setAnimation("neutral_right");
				}
				else if(strcmp("walk_left", active_animation_name) == 0){
					setAnimation("neutral_left");
				}
				else if(strcmp("walk_down", active_animation_name) == 0){
					setAnimation("neutral_down");
				}
			}

			//If (pressing no keys or pressing keys in opposite directions or pressing all keys) and was just walking
			if((keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_D])
			|| (keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_D])){
				setAnimation("walk_up");
				active_animation->setNextAnimation(findAnimation("walk_up"));
			}
			else if((!keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_D])
				 || (!keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_D])){
				setAnimation("walk_down");
				active_animation->setNextAnimation(findAnimation("walk_down"));
			}
			else if((!keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_D])
				  || (keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_D])
				  || (!keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_D])
				  || (keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_D])){
				setAnimation("walk_left");
				active_animation->setNextAnimation(findAnimation("walk_left"));
			}
			else if((!keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_D])
				  || (keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_D])
				  || (!keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_D])
				  || (keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_D])){
				setAnimation("walk_right");
				active_animation->setNextAnimation(findAnimation("walk_right"));
			}

			//Move the actual accel mod to processing?
			if(keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_S]){
				character_control.y_movement = -1;
			}
			else if(keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_W]){
				character_control.y_movement = 1;
			}

			if(keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D]){
				character_control.x_movement = -1;
			}
			else if(keys[SDL_SCANCODE_D] && !keys[SDL_SCANCODE_A]){
				character_control.x_movement = 1;
			}

			if(digital[SDL_SCANCODE_SPACE] && this->z == this->ground){
				character_control.jump = true;
			}
		}
		else if(this->control == CONTROL_TYPE::GAMEPAD && !this->sliding){
			const ControllerState* pad = control->getController(this->control_num);
			double x_axis = pad->left_stick_x_axis;
			double y_axis = pad->left_stick_y_axis;
			//Q1
			if(x_axis == 0.0 && y_axis == 0.0 && this->isWalking()){
				if(strcmp("walk_up", active_animation_name) == 0){
					setAnimation("neutral_right");
				}
				else if(strcmp("walk_right", active_animation_name) == 0){
					setAnimation("neutral_right");
				}
				else if(strcmp("walk_left", active_animation_name) == 0){
					setAnimation("neutral_left");
				}
				else if(strcmp("walk_down", active_animation_name) == 0){
					setAnimation("neutral_down");
				}
			}
			else if(x_axis > 0.0 && y_axis > 0.0){
				if(x_axis > y_axis){
					setAnimation("walk_right");
					active_animation->setNextAnimation(findAnimation("walk_right"));
				}
				else if(x_axis <= y_axis){
					setAnimation("walk_down");
					active_animation->setNextAnimation(findAnimation("walk_down"));
				}
			}
			//Q4
			else if(x_axis > 0.0 && y_axis < 0.0){
				if(x_axis > abs(y_axis)){
					setAnimation("walk_right");
					active_animation->setNextAnimation(findAnimation("walk_right"));
				}
				else if(x_axis <= abs(y_axis)){
					setAnimation("walk_up");
					active_animation->setNextAnimation(findAnimation("walk_up"));
				}
			}
			//Q2
			else if(x_axis < 0.0 && y_axis > 0.0){
				if(abs(x_axis) > y_axis){
					setAnimation("walk_left");
					active_animation->setNextAnimation(findAnimation("walk_left"));
				}
				else if(abs(x_axis) <= y_axis){
					setAnimation("walk_right");
					active_animation->setNextAnimation(findAnimation("walk_right"));
				}
			}
			//Q3
			else if(x_axis < 0.0 && y_axis < 0.0){
				//No abs needed, but more extreme direction will be the lesser
				if(x_axis < y_axis){
					setAnimation("walk_left");
					active_animation->setNextAnimation(findAnimation("walk_left"));
				}
				else if(x_axis >= y_axis){
					setAnimation("walk_down");
					active_animation->setNextAnimation(findAnimation("walk_down"));
				}
			}
			else if(x_axis > 0.0){
				setAnimation("walk_right");
					active_animation->setNextAnimation(findAnimation("walk_right"));
			}
			else if(y_axis > 0.0){
				setAnimation("walk_down");
					active_animation->setNextAnimation(findAnimation("walk_down"));
			}
			else if(x_axis < 0.0){
				setAnimation("walk_left");
				active_animation->setNextAnimation(findAnimation("walk_left"));
			}
			else if(y_axis < 0.0){
				setAnimation("walk_up");
				active_animation->setNextAnimation(findAnimation("walk_up"));
			}

			//If we're not sliding (in an actionable state)
			character_control.y_movement = pad->left_stick_y_axis;
			character_control.x_movement = pad->left_stick_x_axis;
		}
	}
}

void Character::process(uint64_t delta, unsigned int steps){
    if(!Engine::getInstance()->checkState(GAME_STATE::PAUSE | GAME_STATE::DISCUSSION | GAME_STATE::TITLE)){
        //Checking to see if we're still sliding
        if(this->sliding == true){
            if(this->xV + this->yV < 0.5){
                this->sliding = false;
            }
        }
    }

	double x_walk = character_control.x_movement * WALK_SPEED * steps;
	if(xV + x_walk >= WALK_SPEED){
		xV = WALK_SPEED;
	}
	else if(xV + x_walk <= WALK_SPEED * -1){
		xV = WALK_SPEED * -1;
	}
	else{
		xA += character_control.x_movement * WALK_SPEED * steps;
	}

	double y_walk = character_control.y_movement * WALK_SPEED * steps;
	if(yV + y_walk >= WALK_SPEED){
		yV = WALK_SPEED;
	}
	else if(yV + y_walk <= WALK_SPEED * -1){
		yV = WALK_SPEED * -1;
	}
	else{
		yA += character_control.y_movement * WALK_SPEED * steps;
	}

	if(character_control.jump){
		zA += JUMP_SPEED;
	}
}

/** Called on object collision; should be overridden by children if you want collision logic.
 * @param other The other object
 * @param this_hitbox The hitbox that collided from this object
 * @param other_hitbox The hitbox that collided from the other object
 */
void Character::onCollide(Object* other, Hitbox* this_hitbox, Hitbox* other_hitbox){
	//TODO: Make character-specific env collision feel better
	unsigned int other_type = other_hitbox->getType();
	unsigned int this_type = this_hitbox->getType();

	if(this_type & COLLISION){
		if(other_type & COLLISION && other->getType() == EXTENDED_OBJECT_TYPE::BALL){
			double x_force = other_hitbox->getX() - this_hitbox->getX();
			double y_force =  other_hitbox->getY() - this_hitbox->getY();
			Normalize2DVector(&x_force, &y_force);
			other->applyForce(x_force * KICK_SPEED, y_force * KICK_SPEED);
		}
		else if(other_type & COLLISION && other_type & MOVABLE){
			double x_diff = this->x - this->old_x;
			double y_diff = this->y - this->old_y;

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
					//If all else fails, hand it back to the generic collision handling function
					if(this_hitbox->checkCollision(other_hitbox)){
						this->x = new_x;
						this->y = new_y;
					}
				}
			}
		}
	}
}