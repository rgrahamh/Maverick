#include "./Player.hpp"

Player::Player(float start_x, float start_y, RACE race, STYLE style, Stats* stats, Mastery* mastery, Abilities* abilities, Equipment* equipment, InvSlot** inventory, unsigned int draw_layer)
    : Character(start_x, start_y, race, style, stats, mastery, abilities, equipment, inventory, draw_layer){
}