#include "./Player.hpp"

Player::Player(float start_x, float start_y, float friction, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, Equipment* equipment, InvSlot** inventory, unsigned int animation_num)
    : Character(start_x, start_y, friction, race, style, stats, mastery, abilities, equipment, inventory){
    keys.up_arrow = 0;
    keys.down_arrow = 0;
    keys.left_arrow = 0;
    keys.right_arrow = 0;
}

Player::~Player(){}

void Player::process(){
        if(this->keys.up_arrow){
            this->y -= 3;
        }
        if(this->keys.down_arrow){
            this->y += 3;
        }
        if(this->keys.left_arrow){
            this->x -= 3;
        }
        if(this->keys.right_arrow){
            this->x += 3;
        }
}

void Player::action(sf::Event event){
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