#ifndef PLAYER_H
#define PLAYER_H

#include "../Character.hpp"

class Player : public Character{
    public:
        Player(float start_x, float start_y, float friction, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, Equipment* equipment = NULL, InvSlot** inventory = NULL, unsigned int animation_num = 12);
        ~Player();

        void process();
        void action(sf::Event event);

    private:
        typedef struct KeyboardState{
            bool up_arrow;
            bool down_arrow;
            bool left_arrow;
            bool right_arrow;
        } KeyState;

        KeyState keys;
};

#endif