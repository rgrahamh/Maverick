#include "./Player.hpp"

Player::Player(float start_x, float start_y, float friction, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, Equipment* equipment, InvSlot** inventory, unsigned int draw_layer, unsigned int animation_num)
    : Character(start_x, start_y, friction, race, style, stats, mastery, abilities, equipment, inventory, draw_layer, animation_num){
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
            /*if(!this->keys.left_arrow && !this->keys.right_arrow){
                this->setAnimation(1);
            }*/
        }
        if(event.key.code == sf::Keyboard::Down){
            this->keys.down_arrow = 1;
            /*if(!this->keys.left_arrow && !this->keys.right_arrow){
                //this->setAnimation(2);
            }*/
        }
        if(event.key.code == sf::Keyboard::Left){
            this->keys.left_arrow = 1;
            /*if(!this->keys.left_arrow && !this->keys.right_arrow){
                //this->setAnimation(3);
            }*/
        }
        if(event.key.code == sf::Keyboard::Right){
            this->keys.right_arrow = 1;
            /*if(!this->keys.left_arrow && !this->keys.right_arrow){
                //this->setAnimation(4);
            }*/
        }
    }

    if(event.type == sf::Event::KeyReleased){
        if(event.key.code == sf::Keyboard::Up){
            this->keys.up_arrow = 0;
        }
        if(event.key.code == sf::Keyboard::Down){
            this->keys.down_arrow = 0;
        }
        if(event.key.code == sf::Keyboard::Left){
            this->keys.left_arrow = 0;
        }
        if(event.key.code == sf::Keyboard::Right){
            this->keys.right_arrow = 0;
        }
    }
}