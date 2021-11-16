#ifndef CONTROL_H
#define CONTROL_H

#include <SDL2/SDL.h>

#define MAX_GAMEPADS 4
#define MAX_AXIS_RANGE 32767.0
#define STICK_DEADZONE 0.2
#define TRIGGER_DEADZONE 0.05

//Tracks the controller state
struct ControllerState{
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
    float right_trigger_axis;
    float left_trigger_axis;

    //Right stick axies
    float right_stick_x_axis;
    float right_stick_y_axis;

    //Left stick axies
    float left_stick_x_axis;
    float left_stick_y_axis;
};

struct MouseState{
    //Mouse location
    int x;
    int y;

    uint32_t button_state;
};

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
        const uint8_t* getOldKeys() const;
        const uint8_t* getKeys() const;

        const MouseState* getMouse() const;
        const MouseState* getOldMouse() const;

    private:
        //Helper update functions
        int updateControllers();
        void updateKeyboard();
        void updateMouse();

        //The number of connected controllers
        uint8_t num_controllers;

        //All controllers
        SDL_GameController* controller_objs[MAX_GAMEPADS];

        //Controller states for the prior & current frame
        ControllerState old_controllers[MAX_GAMEPADS];
        ControllerState controllers[MAX_GAMEPADS];

        //Keyboard states for the prior & current frame
        uint8_t old_keys[512];
        uint8_t keys[512];

        MouseState mouse;
        MouseState old_mouse;
};

#endif