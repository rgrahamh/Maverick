#ifndef CONTROL_H
#define CONTROL_H

#include <SDL2/SDL.h>
#include <atomic>

static const unsigned int MAX_GAMEPADS = 4;
static const double MAX_AXIS_RANGE = 32767.0;
static const double STICK_DEADZONE = 0.2;
static const double TRIGGER_DEADZONE = 0.05;

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
    int32_t x;
    int32_t y;

    //Mouse buttons
    uint32_t button_state;

    //Scroll wheel
    int32_t scroll_wheel;
};

class Control{
    public:
        /** Default constructor for the Control object
         */
        Control();
        /** Default destructor for the Control object
         */
        ~Control();

        /** Update the controller input (happens once per game loop)
         */
        void updateInput();

        //Gets controller states
        /** Gets the controller state last frame
         * @param controller The controller number
         * @return The controller state last frame
         */
        const ControllerState* getOldController(uint8_t controller) const;

        /** Gets the current controller state
         * @param controller The controller number
         * @return The current controller state
         */
        const ControllerState* getController(uint8_t controller) const;

        //Sets controller states
        /** Gets the digital presses (keys pressed this frame that weren't last frame)
         * @return The digital presses since last frame
         */
        const uint8_t* getDigitalPress() const;

        /** Gets the keyboard state last frame
         * @return The keyboard state last frame
         */
        const uint8_t* getOldKeys() const;

        /** Gets the current keyboard state
         * @return The current keyboard state
         */
        const uint8_t* getKeys() const;

        /** Gets the mouse state last frmae
         * @return The mouse state last frame
         */
        const MouseState* getMouse() const;

        /** Gets the mouse state last frmae
         * @return The mouse state last frame
         */
        const MouseState* getOldMouse() const;

    private:
        //Helper update functions
        /** Update controller states
         * @return number of controllers
         */
        int updateControllers();

        /** Update keyboard state
         */
        void updateKeyboard();

        /** Update mouse state
         */
        void updateMouse();

        /** Polls events (for updating input that isn't captured in other SDL operations)
         */
        void pollEvents();

        //The number of connected controllers
        uint8_t num_controllers;

        //All controllers
        SDL_GameController* controller_objs[MAX_GAMEPADS];

        //Controller states for the prior & current frame
        ControllerState old_controllers[MAX_GAMEPADS];
        ControllerState controllers[MAX_GAMEPADS];

        //Keyboard states for the prior & current frame
        uint8_t digital_press[512];
        uint8_t old_keys[512];
        uint8_t keys[512];

        MouseState mouse;
        MouseState old_mouse;
};

#endif