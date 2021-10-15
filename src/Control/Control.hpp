#ifndef CONTROL_H
#define CONTROL_H

#include <SDL2/SDL.h>

#define MAX_GAMEPADS 4

//Tracks the keyboard state
typedef struct KeyState{
    uint8_t keys[512];
    uint16_t num_keys;
} KeyState;

//Tracks the controller state
typedef struct ControllerState{
    //Face buttons
    uint8_t A;
    uint8_t B;
    uint8_t X;
    uint8_t Y;

    //Bumpers
    uint8_t right_bumper;
    uint8_t left_bumper;

    //Aux. Buttons
    uint8_t start;
    uint8_t select;

    //D-Pad
    uint8_t up;
    uint8_t down;
    uint8_t left;
    uint8_t right;

    //Stick buttons
    uint8_t right_stick_button;
    uint8_t left_stick_button;

    //Triggers
    int16_t right_trigger_axis;
    int16_t left_trigger_axis;

    //Right stick axies
    int16_t right_stick_x_axis;
    int16_t right_stick_y_axis;

    //Left stick axies
    int16_t left_stick_x_axis;
    int16_t left_stick_y_axis;

} ControllerState;

class Control{
    public:
        Control();
        ~Control();

        //Updates the input per frame (Engine should call this function)
        void updateInput();

        //Gets controller states
        const ControllerState* getOldController(uint8_t controller) const;
        const ControllerState* getController(uint8_t controller) const;

        //Sets controller states
        const KeyState* getOldKeys() const;
        const KeyState* getKeys() const;

    private:
        //Helper update functions
        int updateControllers();
        void updateKeyboard();

        //The number of connected controllers
        uint8_t num_controllers;

        //All controllers
        SDL_GameController* controller_objs[MAX_GAMEPADS];

        //Controller states for the prior & current frame
        ControllerState old_controllers[MAX_GAMEPADS];
        ControllerState controllers[MAX_GAMEPADS];

        //Keyboard states for the prior & current frame
        KeyState old_keys;
        KeyState keys;
};

#endif