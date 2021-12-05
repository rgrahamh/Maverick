#include "./Character.hpp"
#include "../../../Engine/Engine.hpp"

class Engine;
extern Engine* engine;

Character::Character(const char* name, float start_x, float start_y, float friction, float mass, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, CONTROL_TYPE control, Equipment* equipment, InvSlot** inventory, unsigned animation_num, int draw_layer)
	: Object(name, start_x, start_y, friction, mass, animation_num, draw_layer),
	Race(race),
	Style(style){
	//Setting the struct pointers; these are the non-optional params
	this->stats = stats;
	this->mastery = mastery;
	this->abilities = abilities;
	this->control = control;
    this->sliding = false;
	this->control = CONTROL_TYPE::GAMEPAD | CONTROL_TYPE::KEYBOARD;
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

Character::~Character(){
	free(this->inventory);
	delete this->equipment;
}

bool Character::isStanding(){
    return active_animation >= UP_NEUTRAL && active_animation <= RIGHT_NEUTRAL;
}

bool Character::isWalking(){
    return active_animation >= UP_WALK && active_animation <= RIGHT_WALK;
}

/** Calculates any actions taken; should be overridden by children if used
 * @param event The event being interpreted
 */
void Character::action(Control* control){
    const uint8_t* keys = control->getKeys();
	const ControllerState* pad = control->getController(this->control_num);

    if(!engine->checkState(GAME_STATE::PAUSE | GAME_STATE::DISCUSSION | GAME_STATE::TITLE)){
		//If the character is in an actionable state
		if(this->control & CONTROL_TYPE::KEYBOARD){
			//If (pressing no keys or pressing keys in opposite directions or pressing all keys) and was just walking
			if(((!(keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_D]))
			|| (keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_D])
			|| (!keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_D])
			|| (keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_D]))
			&& this->isWalking()){
				setAnimation(active_animation - 4);
			}
			//Else if up and no other keys
			else if(keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_D]){
				setAnimation(UP_WALK);
			}
			else if(!keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_D]){
				setAnimation(DOWN_WALK);
			}
			else if((!keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_D])
				  || (keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_D])
				  || (!keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_D])
				  || (keys[SDL_SCANCODE_W] && keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_S] && !keys[SDL_SCANCODE_D])){
				setAnimation(LEFT_WALK);
			}
			else if((!keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_D])
				  || (keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_D])
				  || (!keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_D])
				  || (keys[SDL_SCANCODE_W] && !keys[SDL_SCANCODE_A] && keys[SDL_SCANCODE_S] && keys[SDL_SCANCODE_D])){
				setAnimation(RIGHT_WALK);
			}
		}
		else if(this->control & CONTROL_TYPE::GAMEPAD){
			float x_axis = pad->left_stick_x_axis;
			float y_axis = pad->left_stick_y_axis;
			if(x_axis == 0.0 && y_axis == 0.0 && this->isWalking()){
				setAnimation(active_animation - 4);
			}
			else if(x_axis > 0.0 && y_axis > 0.0){
				if(x_axis > y_axis && this->active_animation != RIGHT_WALK){
					setAnimation(RIGHT_WALK);
				}
				else if(x_axis <= y_axis && this->active_animation != DOWN_WALK){
					setAnimation(DOWN_WALK);
				}
			}
			else if(x_axis > 0.0 && y_axis < 0.0){
				if(x_axis > abs(y_axis) && this->active_animation != RIGHT_WALK){
					setAnimation(RIGHT_WALK);
				}
				else if(x_axis <= abs(y_axis) && this->active_animation != UP_WALK){
					setAnimation(UP_WALK);
				}
			}
			else if(x_axis < 0.0 && y_axis > 0.0){
				if(abs(x_axis) > y_axis && this->active_animation != LEFT_WALK){
					setAnimation(LEFT_WALK);
				}
				else if(abs(x_axis) <= y_axis && this->active_animation != DOWN_WALK){
					setAnimation(DOWN_WALK);
				}
			}
			else if(x_axis < 0.0 && y_axis < 0.0){
				//No abs needed, but more extreme direction will be the lesser
				if(x_axis < y_axis && this->active_animation != LEFT_WALK){
					setAnimation(LEFT_WALK);
				}
				else if(x_axis >= y_axis && this->active_animation != UP_WALK){
					setAnimation(UP_WALK);
				}
			}
			else if(x_axis > 0.0 && this->active_animation != RIGHT_WALK){
				setAnimation(RIGHT_WALK);
			}
			else if(y_axis > 0.0 && this->active_animation != DOWN_WALK){
				setAnimation(DOWN_WALK);
			}
			else if(x_axis < 0.0 && this->active_animation != LEFT_WALK){
				setAnimation(LEFT_WALK);
			}
			else if(y_axis < 0.0 && this->active_animation != UP_WALK){
				setAnimation(UP_WALK);
			}
		}

		//If we're not sliding (in an actionable state)
		if(!this->sliding){
			if(this->control & CONTROL_TYPE::KEYBOARD){
				if(keys[SDL_SCANCODE_W]){
					this->yA -= 0.1;
				}
				if(keys[SDL_SCANCODE_S]){
					this->yA += 0.1;
				}
				if(keys[SDL_SCANCODE_A]){
					this->xA -= 0.1;
				}
				if(keys[SDL_SCANCODE_D]){
					this->xA += 0.1;
				}
			}
			else if(this->control & CONTROL_TYPE::GAMEPAD){
				this->yA += pad->left_stick_y_axis * 0.1;
				this->xA += pad->left_stick_x_axis * 0.1;
			}
		}
	}
}

void Character::process(uint32_t delta){
    if(!engine->checkState(GAME_STATE::PAUSE | GAME_STATE::DISCUSSION | GAME_STATE::TITLE)){
        //Checking to see if we're still sliding
        if(this->sliding == true){
            if(this->xV + this->yV < 1){
                this->sliding = false;
            }
        }
    }
}

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