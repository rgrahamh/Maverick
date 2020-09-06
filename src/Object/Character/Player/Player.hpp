#ifndef PLAYER_H
#define PLAYER_H

#include "../Character.hpp"

class Player : public Character{
    public:
        Player(float start_x, float start_y, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, Equipment* equipment, InvSlot** inventory, unsigned int draw_layer, unsigned int animation_num);
        ~Player();

        void process();
        void action(sf::Event event);
};

#endif