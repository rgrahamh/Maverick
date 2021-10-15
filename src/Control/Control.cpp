#include "./Control.hpp"

/** Default constructor for the Control object
 */
Control::Control(){
    old_keys.num_keys = 0;
    keys.num_keys = 0;

    memset(keys.keys, '\0', 512);
    memset(old_keys.keys, '\0', 512);

    //Try to init controllers
    for(int i = 0; i < MAX_GAMEPADS; i++){
        if(SDL_IsGameController(i)){
            controller_objs[i] = SDL_GameControllerOpen(i);
        }

        memset(&controllers[i], '\0', sizeof(ControllerState));
        memset(&old_controllers[i], '\0', sizeof(ControllerState));
    }
}

/** Default destructor for the Control object
 */
Control::~Control(){
    for(int i = 0; i < MAX_GAMEPADS; i++){
        if(controller_objs[i] != nullptr){
            SDL_GameControllerClose(controller_objs[i]);
        }
    }
}

/** Update controller states
 * @return number of controllers
 */
int Control::updateControllers(){
    uint8_t n_controllers = 0;
    for(int i = 0; i < MAX_GAMEPADS; i++){
        //Try to connect new controllers
        if(controller_objs[i] == nullptr && SDL_IsGameController(i)){
            controller_objs[i] = SDL_GameControllerOpen(i);
        }

        if(SDL_IsGameController(i) && controller_objs[i] != nullptr && SDL_GameControllerGetAttached(controller_objs[i])){
            n_controllers++;

            //Using the default copy contructor, copy the struct
            old_controllers[i] = controllers[i];

            controllers[i].A = SDL_GameControllerGetButton(controller_objs[i], SDL_CONTROLLER_BUTTON_A);
            controllers[i].B = SDL_GameControllerGetButton(controller_objs[i], SDL_CONTROLLER_BUTTON_B);
            controllers[i].X = SDL_GameControllerGetButton(controller_objs[i], SDL_CONTROLLER_BUTTON_X);
            controllers[i].Y = SDL_GameControllerGetButton(controller_objs[i], SDL_CONTROLLER_BUTTON_Y);

            controllers[i].right_bumper = SDL_GameControllerGetButton(controller_objs[i], SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
            controllers[i].left_bumper = SDL_GameControllerGetButton(controller_objs[i], SDL_CONTROLLER_BUTTON_LEFTSHOULDER);

            controllers[i].start = SDL_GameControllerGetButton(controller_objs[i], SDL_CONTROLLER_BUTTON_START);
            controllers[i].select = SDL_GameControllerGetButton(controller_objs[i], SDL_CONTROLLER_BUTTON_BACK);

            controllers[i].up = SDL_GameControllerGetButton(controller_objs[i], SDL_CONTROLLER_BUTTON_DPAD_UP);
            controllers[i].down = SDL_GameControllerGetButton(controller_objs[i], SDL_CONTROLLER_BUTTON_DPAD_DOWN);
            controllers[i].left = SDL_GameControllerGetButton(controller_objs[i], SDL_CONTROLLER_BUTTON_DPAD_LEFT);
            controllers[i].right = SDL_GameControllerGetButton(controller_objs[i], SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

            controllers[i].right_stick_button = SDL_GameControllerGetButton(controller_objs[i], SDL_CONTROLLER_BUTTON_RIGHTSTICK);
            controllers[i].left_stick_button = SDL_GameControllerGetButton(controller_objs[i], SDL_CONTROLLER_BUTTON_LEFTSTICK);

            controllers[i].right_trigger_axis = SDL_GameControllerGetAxis(controller_objs[i], SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
            controllers[i].left_trigger_axis = SDL_GameControllerGetAxis(controller_objs[i], SDL_CONTROLLER_AXIS_TRIGGERLEFT);

            controllers[i].right_stick_x_axis = SDL_GameControllerGetAxis(controller_objs[i], SDL_CONTROLLER_AXIS_RIGHTX);
            controllers[i].right_stick_y_axis = SDL_GameControllerGetAxis(controller_objs[i], SDL_CONTROLLER_AXIS_RIGHTY);

            controllers[i].left_stick_x_axis = SDL_GameControllerGetAxis(controller_objs[i], SDL_CONTROLLER_AXIS_LEFTX);
            controllers[i].left_stick_y_axis = SDL_GameControllerGetAxis(controller_objs[i], SDL_CONTROLLER_AXIS_LEFTY);
        }
    }

    return n_controllers;
}

/** Update keyboard state
 */
void Control::updateKeyboard(){
    int num_elements;
    const uint8_t* keyboard = SDL_GetKeyboardState(&num_elements);

    //Update the old keyboard state w/ the last keyboard state
    memcpy(old_keys.keys, keys.keys, old_keys.num_keys);
    old_keys.num_keys = keys.num_keys;

    //Update the new keyboard state w/ the newly-polled information
    memcpy(keys.keys, keyboard, num_elements);
    keys.num_keys = num_elements;
}

/** Update the controller input (happens once per game loop)
 */
void Control::updateInput(){
    //Poll all events
    SDL_GameControllerUpdate();
    SDL_PumpEvents();

    //Update controllers
    num_controllers = updateControllers();

    //Update keyboard
    updateKeyboard();
}

/** Gets the controller state last frame
 * @param controller The controller number
 * @return The controller state last frame
 */
const ControllerState* Control::getOldController(uint8_t controller) const{
    return &old_controllers[controller];
}

/** Gets the current controller state
 * @param controller The controller number
 * @return The current controller state
 */
const ControllerState* Control::getController(uint8_t controller) const{
    return &controllers[controller];
}

/** Gets the keyboard state last frame
 * @return The keyboard state last frame
 */
const KeyState* Control::getOldKeys() const{
    return &old_keys;
}

/** Gets the current keyboard state
 * @return The current keyboard state
 */
const KeyState* Control::getKeys() const{
    return &keys;
}