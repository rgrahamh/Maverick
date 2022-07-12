#include "./Engine.hpp"

// EVENTUALLY, we'll want to take out this include and replace the "loadZone()" with a proper zone loading function
#include "../Zone/ZoneFactory/ZoneFactory.hpp"
#include "../FileHandler/Saver/Saver.hpp"
#include "../FileHandler/Loader/Loader.hpp"

std::atomic<bool> exit_game;

bool endian;
bool debug = false;

std::thread::id base_thread_id = std::this_thread::get_id();

Engine* engine;

/** Engine's parameterized constructor
 * @param zones The zones that the game engine is initialized with
 */
Engine::Engine(){
    exit_game = false;

    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING)){
        printf("Failed to init everything!\n");
        exit(-1);
    }
    if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG){
        printf("Failed to init IMG!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
        exit(-1);
    }
    //Don't need to init w/ anything else; we're just using WAVs
    if(Mix_Init(MIX_INIT_MP3) < 0){
        printf("Failed to init Mixer! ERR: %s\n", Mix_GetError());
        exit(-1);
    }

    //Play around w/ the audio frequency
    if(Mix_OpenAudio(44100, AUDIO_S32LSB, 2, 512) < 0){
        printf("Failed to init Mixer Audio! ERR: %s\n", Mix_GetError());
        exit(-1);
    }

    for(int i = 0; i < SDL_GetNumAudioDevices(0); i++){
        printf("Audio device %i: %s\n", i, SDL_GetAudioDeviceName(i, 0));
    }

    //Initialization of the sound board
    this->sound_board = new SoundBoard();

    //Initialization of control system
    control = new Control();

	this->window = SDL_CreateWindow("Cyberena", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if(window == nullptr){
        printf("Could not create window; exiting");
        fflush(stdout);
        exit(-1);
    }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    int win_width, win_height;
    SDL_GetWindowSize(this->window, &win_width, &win_height);

    //Get rid of SDL_RENDERER_PRESENTVSYNC if we want to take the frame cap off
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL){
        printf("Renderer is null; exiting");
        fflush(stdout);
        exit(-1);
    }
    this->camera = new Camera(renderer, window, nullptr, CAMERA_FOLLOW_MODE::FIXED_FOLLOW, 0.08);

    //Set up the screenshot blit surface
    this->screen_blit_surface = SDL_CreateRGBSurface(0, win_width, win_height, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    this->screen_blit_texture = nullptr;

    //Setting up the hash tables
    this->sprite_hash = new SpriteHash(2048);
    this->sound_hash = new SoundHash(2048);
    this->music_hash = new MusicHash(2048);
    this->font_hash = new FontHash(2048);

    //Set scales
    //We want to scale both X & Y by the Y element, so stuff doesn't get squashed
    this->native_x_scale = win_height / BASE_SCREEN_HEIGHT;
    this->native_y_scale = win_height / BASE_SCREEN_HEIGHT;
    this->current_x_scale = native_x_scale;
    this->current_y_scale = native_y_scale;
    this->target_x_scale = native_x_scale;
    this->target_y_scale = native_y_scale;

    this->camera->setScale(this->native_x_scale, this->native_y_scale);

    this->zones = NULL;
    this->active_zones = NULL;

    //Set to the title screen
    this->state = GAME_STATE::OVERWORLD;

    this->entities.obj = nullptr;
    this->entities.ui = nullptr;

    this->delta = SDL_GetTicks(); //Upgrade to SDL_GetTicks64 once Fedora upgrades its packages

    this->gravity = 0.1;

    endian = getEndian();

    this->thread_cleanup_queue = new std::queue<std::thread::id>();
}

/** Engine's destructor
 */
Engine::~Engine(){
    ZoneList* zone_cursor = zones;
    while(zones != NULL){
        zone_cursor = zones->next;
        delete zones->zone;
        delete zones;
        zones = zone_cursor;
    }

    zone_cursor = active_zones;
    while(active_zones != NULL){
        zone_cursor = active_zones->next;
        delete active_zones->zone;
        delete active_zones;
        active_zones = zone_cursor;
    }

    SDL_DestroyRenderer(camera->getRenderer());
    SDL_DestroyWindow(window);
    delete camera;
    IMG_Quit();
    SDL_Quit();
    Mix_CloseAudio();
    Mix_Quit();

    freeFullEntityList();
}

/** The function that is called to start the game engine's operation
 */
void Engine::start(){
    //loadZoneFromFile("global");
    //loadZoneFromFile("Test Zone");

    //Loading the test zone as the first area
    //this->addThread(new std::thread(loadZone, "global"));
    loadZone("global");
    //loadZone("led");

    //Loading the test zone as the first area
    //this->addThread(new std::thread(loadZone, "Test Zone"));
    loadZone("Test Zone");

    //Loading the level editor
    //engine->addThread(new std::thread(loadZone, "led"));

    //saveZone(this->getZone("global"));
    //saveZone(this->getZone("Test Zone"));

    Music* song1 = new Music("song1");
    Sound* sound = new Sound;
    sound->name = "bass";
    sound->sample = Mix_LoadWAV("./assets/audio/music/bass_riff_idea.wav");
    song1->addTrack(sound);
    engine->addMusic("song1", song1);
    sound_board->playMusic("song1");
    sound_board->setMusicVolume(1, 1.0, 10000);

    gameLoop();
}

/** The primary game loop
 */
void Engine::gameLoop(){
    uint64_t last_time = this->delta;

    uint64_t fps = 0;
    uint64_t fps_counter = 0;
    uint64_t physics_step_accumulator = 0;
    delta = SDL_GetTicks();
	while(!exit_game){
        buildFullEntityList();

        //Calculate the time that has passed since last frame
        delta = SDL_GetTicks() - last_time;
        last_time = SDL_GetTicks();

        physics_step_accumulator += delta;
        uint64_t physics_step = physics_step_accumulator / PHYSICS_STEP_SIZE;
        physics_step_accumulator %= PHYSICS_STEP_SIZE;

        //No need to do anything if a step hasn't occurred
        if(delta > 0){
            fps_counter += delta;
            fps++;
            if(fps_counter >= 1000){
                printf("FPS: %ld\n", fps);
                fps_counter = 0;
                fps = 0;
            }

            if(!(this->state & GAME_STATE::PAUSE) && !(this->state & GAME_STATE::HALT) && physics_step > 0){
                //Cap the physics step at 5 steps (to avoid bound exploitation)
                if(physics_step > 5){
                    physics_step = 5;
                }

                //Action step (where actions occur)
                this->actionStep(&this->entities);

                //Physics step (where physics are calculated, obj positions updated, etc.)
                this->physicsStep(&this->entities, physics_step);

                //Collision step (where collision is determined)
                this->collisionStep(this->entities.obj);
            }

            //Different because we need to adjust the object list for draw order
            this->drawStep(&this->entities);
        }

        //Thread cleanup
        this->threadGC();
	}
}

/** The action step of the game engine
 * @param all_objects All of the objects that should be listening for input
 */
void Engine::actionStep(EntityList* all_entities){
    //Update controller/keyboard input
    control->updateInput();

    ObjectList* obj_cursor = all_entities->obj;
    while(obj_cursor != NULL){
        Object* curr_object = obj_cursor->obj;
        if(unlikely(curr_object->isActive() == false)){
            obj_cursor = obj_cursor->next;
            continue;
        }

        obj_cursor->obj->action(control);
        obj_cursor = obj_cursor->next;
    }

    UIElementList* ui_cursor = all_entities->ui;
    while(ui_cursor != NULL){
        UIElement* curr_element = ui_cursor->element;
        if(unlikely(curr_element->isActive() == false)){
            ui_cursor = ui_cursor->next;
            continue;
        }

        ui_cursor->element->action(control);
        ui_cursor = ui_cursor->next;
    }
    this->globalAction();
}

/** Handles object-nonspecific actions (like menuing for example)
 */
void Engine::globalAction(){
    const uint8_t* keys = control->getKeys();
    const uint8_t* old_keys = control->getOldKeys();
    if(keys[SDL_SCANCODE_ESCAPE] && !old_keys[SDL_SCANCODE_ESCAPE]){
        if(this->checkState(GAME_STATE::PAUSE)){
            this->getUIElement("pause_menu")->setVisible(false);
            this->getUIElement("pause_menu")->setActive(false);
            this->state &= ~GAME_STATE::PAUSE;
        }
        // You may pause in the overworld or in battle
        else if(this->checkState(GAME_STATE::OVERWORLD | GAME_STATE::BATTLE)){
            this->getUIElement("pause_menu")->setVisible(true);
            this->getUIElement("pause_menu")->setActive(true);
            this->state |= GAME_STATE::PAUSE;
        }
    }
}

/** The physics step of the game engine
 * @param all_objects All of the objects that physics should be simluated for
 */
void Engine::physicsStep(EntityList* all_entities, unsigned int steps){
    ObjectList* all_objects = all_entities->obj;
    if(!this->checkState(GAME_STATE::PAUSE)){
        while(all_objects != nullptr){
            Object* curr_object = all_objects->obj;
            if(unlikely(curr_object->isActive() != false)){
                all_objects->obj->_process(this->delta, steps);
            }
            all_objects = all_objects->next;
        }
    }

    UIElementList* all_elements = all_entities->ui;
    while(all_elements != nullptr){
        UIElement* curr_element = all_elements->element;
        if(unlikely(curr_element->isActive() != false)){
            all_elements->element->_process(this->delta, steps);
        }
        all_elements = all_elements->next;
    }

    this->current_x_scale += (this->target_x_scale - this->current_x_scale) * ZOOM_RATE;
    this->current_y_scale += (this->target_y_scale - this->current_y_scale) * ZOOM_RATE;
}

/** The collision step of the game engine
 * @param all_objects All of the objects that collisions should be checked for
 */
void Engine::collisionStep(ObjectList* all_objects){
    //Sister arrays for the list matricies
    ObjectList* object_matrix[16][9] = {NULL};
    HitboxList* hitbox_matrix[16][9] = {NULL};

    //While we're not out of objects
    while(all_objects != NULL){
        Object* curr_object = all_objects->obj;
        if(unlikely(curr_object->isActive() == false)){
            all_objects = all_objects->next;
            continue;
        }
        HitboxList* object_hitboxes = all_objects->obj->getHitboxes();

        //While we're not out of object hitboxes
        while(object_hitboxes != NULL){
            float top_bound, bot_bound, left_bound, right_bound;
            Hitbox* curr_hitbox = object_hitboxes->hitbox;

            //Set up the bounds
            switch(curr_hitbox->getShape()){
                case HIT_RECT:{
                        HitRect* rect_hitbox = (HitRect*)curr_hitbox;

                        top_bound = rect_hitbox->getY();
                        bot_bound = rect_hitbox->getY() + rect_hitbox->getHeight();
                        left_bound = rect_hitbox->getX();
                        right_bound = rect_hitbox->getX() + rect_hitbox->getWidth();
                    }
                    break;

                case HIT_ELLIPSE:{
                        HitEllipse* ellipse_hitbox = (HitEllipse*)curr_hitbox;

                        top_bound = ellipse_hitbox->getY() - ellipse_hitbox->getYRadius();
                        bot_bound = ellipse_hitbox->getY() + ellipse_hitbox->getYRadius();
                        left_bound = ellipse_hitbox->getX() - ellipse_hitbox->getXRadius();
                        right_bound = ellipse_hitbox->getX() + ellipse_hitbox->getXRadius();
                    }
                    break;

                case HIT_CONE:{
                        HitCone* cone_hitbox = (HitCone*)curr_hitbox;

                        top_bound = cone_hitbox->getY() - cone_hitbox->getYRadius();
                        bot_bound = cone_hitbox->getY() + cone_hitbox->getYRadius();
                        left_bound = cone_hitbox->getX() - cone_hitbox->getXRadius();
                        right_bound = cone_hitbox->getX() + cone_hitbox->getXRadius();
                    }
                    break;
            }
            
            //The number of blocks in x & y directions
            const int x_block = 16, y_block = 9;

            int win_width, win_height;
            SDL_GetWindowSize(this->window, &win_width, &win_height);
            int x_iter = win_width * 2 / x_block;
            int y_iter = win_height * 2 / y_block;
            int x_min = camera->getX() - (win_width / 2); 
            int y_min = camera->getY() - (win_height / 2); 
            int x_max = win_width * 1.5 + camera->getX();
            int y_max = win_height * 1.5 + camera->getY();
            int j = 0;
            //Set up the object & hitbox matricies
            //Go by height as the outer loop since it eliminates the most
            if(x_iter > 0 && y_iter > 0){
                for(int box_y = y_min; box_y < y_max; box_y += y_iter){
                    //If the top_bound is below box_y + win_height or if the bot_bound is above box_y, go to next
                    if(!(top_bound > box_y + y_iter || bot_bound < box_y)){
                        int i = 0;
                        for(int box_x = x_min; box_x < x_max; box_x += x_iter){
                            //If the left bound is greater than box_x or the right bound is less than box_x, go to next
                            if(!(left_bound > box_x + x_iter || right_bound < box_x)){
                                //Insert the object in the list
                                ObjectList* new_objectlst = new ObjectList;
                                new_objectlst->next = object_matrix[i][j];
                                new_objectlst->obj = curr_object;
                                object_matrix[i][j] = new_objectlst;

                                //Insert the hitbox in the list
                                HitboxList* new_hitboxlst = new HitboxList;
                                new_hitboxlst->next = hitbox_matrix[i][j];
                                new_hitboxlst->hitbox = curr_hitbox;
                                hitbox_matrix[i][j] = new_hitboxlst;
                            }
                            i++;
                        }
                    }
                    j++;
                }
            }
            object_hitboxes = object_hitboxes->next;
        }
        all_objects = all_objects->next;
    }

    //For each vertical slice
    for(int i = 0; i < 16; i++){
        //For each horizontal slice
        for(int j = 0; j < 9; j++){
            //Iterate over every item in the list
            ObjectList* object_lst = object_matrix[i][j];
            HitboxList* hitbox_lst = hitbox_matrix[i][j];
            while(object_lst != nullptr && hitbox_lst != nullptr){
                //Iterate over every item after the hitbox_lst
                ObjectList* object_cursor = object_lst->next;
                HitboxList* hitbox_cursor = hitbox_lst->next;
                while(object_cursor != nullptr && hitbox_cursor != nullptr){
                    //Possible place for multi-threading!

                    //If both aren't environment and they collide, and the object isn't the same
                    if(object_lst->obj != object_cursor->obj && (!((hitbox_lst->hitbox->getType() & ENVIRONMENT) && (hitbox_cursor->hitbox->getType() & ENVIRONMENT))) &&
                       object_lst->obj->getCollisionLayer() == object_cursor->obj->getCollisionLayer() && hitbox_lst->hitbox->checkCollision(hitbox_cursor->hitbox)){
                        //We want the default collision handling to go last since it's the harshest (and might inhibit special collision cases)
                        if(!object_lst->obj->checkHitboxImmunity(object_cursor->obj, hitbox_cursor->hitbox) &&
                           !object_cursor->obj->checkHitboxImmunity(object_lst->obj, hitbox_lst->hitbox)){
                            //Handle the base object collision & hitbox immunity
                            object_cursor->obj->onCollide(object_lst->obj, hitbox_cursor->hitbox, hitbox_lst->hitbox);
                            object_cursor->obj->addHitboxImmunity(object_lst->obj, hitbox_lst->hitbox);

                            //Handle the other object collision & hitbox immunity
                            object_lst->obj->onCollide(object_cursor->obj, hitbox_lst->hitbox, hitbox_cursor->hitbox);
                            object_lst->obj->addHitboxImmunity(object_cursor->obj, hitbox_cursor->hitbox);

                            //Handle default collision
                            handleDefaultCollision(object_cursor->obj, hitbox_cursor->hitbox, object_lst->obj, hitbox_lst->hitbox);
                        }
                    }
                    object_cursor = object_cursor->next;
                    hitbox_cursor = hitbox_cursor->next;
                }

                //Linked list cleanup
                ObjectList* tmp_object = object_lst;
                object_lst = object_lst->next;
                delete tmp_object;

                HitboxList* tmp_hitbox = hitbox_lst;
                hitbox_lst = hitbox_lst->next;
                delete tmp_hitbox;
            }
        }
    }
}

/** The draw step of the game engine
 * @param all_entities An entity list of all entities to draw
 */
void Engine::drawStep(EntityList* all_entities){
    SDL_Renderer* renderer = this->camera->getRenderer();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    camera->setScale(current_x_scale, current_y_scale);

    //Draw operation
    all_entities->obj = this->drawSort(all_entities->obj);
    this->camera->_draw(all_entities->obj, this->delta);

    camera->setScale(1.0, 1.0);

    all_entities->ui = (UIElementList*)this->drawSort((ObjectList*)all_entities->ui);
    this->camera->_draw(all_entities->ui, this->delta);

    SDL_RenderPresent(renderer);
}

inline static bool checkDrawOverlap(Object* obj1, Object* obj2){
    return (obj1->getLowerDrawAxis() >= obj2->getUpperDrawAxis() && obj1->getLowerDrawAxis() <= obj2->getLowerDrawAxis()) ||
           (obj1->getUpperDrawAxis() >= obj2->getUpperDrawAxis() && obj1->getUpperDrawAxis() <= obj2->getLowerDrawAxis()) ||
           (obj2->getLowerDrawAxis() >= obj1->getUpperDrawAxis() && obj2->getLowerDrawAxis() <= obj1->getLowerDrawAxis()) ||
           (obj2->getUpperDrawAxis() >= obj1->getUpperDrawAxis() && obj2->getUpperDrawAxis() <= obj1->getLowerDrawAxis());
}

/** Recursively sorts the objects in the order of draw
 * @param curr_obj The current object that you're sorting through
 * @return The current draw object
 */
ObjectList* Engine::drawSort(ObjectList* curr_obj){
    //Checking if we have any objects
    if(curr_obj != NULL){
        //Case where it's the last iteration (everything is sorted)
        if(curr_obj->next == NULL){
            return curr_obj;
        }
        else{
            ObjectList* next_obj = this->drawSort(curr_obj->next);
            Object* nxt = next_obj->obj;
            Object* cur = curr_obj->obj;
            //Higher draw axis = closer to the bottom of the screen (since Y coordinates trend positively downards)
            //If next is a lower draw layer, always move it back in the draw ordering
            if(nxt->getDrawLayer() < cur->getDrawLayer() ||
               //Make sure we're on the same draw layer for behind/overlap case
               (nxt->getDrawLayer() == cur->getDrawLayer() &&
               //Behind case
               (nxt->getLowerDrawAxis() < cur->getUpperDrawAxis() ||
               //Overlap case
               ((checkDrawOverlap(nxt, cur) &&
               (nxt->getZ() < cur->getZ() || (nxt->getZ() == cur->getZ() && nxt->getLowerDrawAxis() < cur->getLowerDrawAxis()))))))){
                //Swap node positions & send curr_obj up the draw chain
                curr_obj->next = next_obj->next;
                next_obj->next = this->drawSort(curr_obj);
                return next_obj;
                //If nodes don't get sunk in the list, does the draw chain break?
            }
            else{
                curr_obj->next = next_obj;
                return curr_obj;
            }
        }
    }
    else{
        return NULL;
    }
}

/** Gets the first object by name in any zone (slower than specifying zone)
 * @param element_name The name of the object element
 * @return A pointer to the object, or NULL if it can't be found
 */
UIElement* Engine::getUIElement(const char* element_name){
    ZoneList* zone_cursor = this->active_zones;
    while(zone_cursor != NULL){
        UIElementList* ui_cursor = zone_cursor->zone->getUIElements();
        while(ui_cursor != NULL && ui_cursor->element != NULL){
            if(strcmp(ui_cursor->element->getName(), element_name) == 0){
                return ui_cursor->element;
            }
            ui_cursor = ui_cursor->next;
        }
        zone_cursor = zone_cursor->next;
    }

    return NULL;
}

/** Gets the first object by name in any zone (slower than specifying zone)
 * @param obj_name The name of the object
 * @return A pointer to the object, or NULL if it can't be found
 */
Object* Engine::getObject(const char* obj_name){
    ZoneList* zone_cursor = this->active_zones;
    while(zone_cursor != NULL){
        ObjectList* obj_cursor = zone_cursor->zone->getObjects();
        while(obj_cursor != NULL && obj_cursor->obj != NULL){
            if(strcmp(obj_cursor->obj->getName(), obj_name) == 0){
                return obj_cursor->obj;
            }
            obj_cursor = obj_cursor->next;
        }
        zone_cursor = zone_cursor->next;
    }

    return NULL;
}

/** Gets the first object found by name from a specific zone
 * @param obj_name The name of the object
 * @param zone_name The name of the zone
 * @return A pointer to the object
 */
Object* Engine::getObject(const char* obj_name, const char* zone_name){
    ZoneList* zone_cursor = this->active_zones;
    while(zone_cursor != NULL){
        if(!strcmp(zone_cursor->zone->getName(), zone_name)){
            ObjectList* obj_cursor = zone_cursor->zone->getObjects();
            while(obj_cursor != NULL){
                if(strcmp(obj_cursor->obj->getName(), obj_name)){
                    return obj_cursor->obj;
                }
            }
        }
        zone_cursor = zone_cursor->next;
    }

    return NULL;
}

/** Gets the state of the engine
 * @return The state of the engine
 */
uint64_t Engine::getState(){
    return this->state;
}

/** Gets the camera engine's camera
 * @return The engine's camera
 */
Camera* Engine::getCamera(){
    return this->camera;
}

/** Gets the window the engine is using
 * @return The window the engine is using
 */
SDL_Window* Engine::getWindow(){
    return this->window;
}

/** Gets the texture representing all object (to be blit elsewhere on the screen)
 * @return The texture being blit (or nullptr if the blit failed)
 */
SDL_Texture* Engine::getScreenBlitTexture(){
    return this->screen_blit_texture;
}

/** Checks the state of the engine against the passed-in state(s)
 * @param chk_state The state(s) you wish to check
 * @return If the engine is in chk_state
 */
bool Engine::checkState(uint64_t chk_state){
    return (this->state & chk_state) != 0;
}

/** Sets the state of the engine
 * @param state The new of the engine
 */
void Engine::setState(uint64_t state){
    this->state = state;
}

/** Sets the global X scale of the engine
 * @param x_scale The global X scale of the engine
 */
void Engine::setGlobalXScale(double x_scale){
    this->target_x_scale = x_scale;
}

/** Sets the global Y scale of the engine
 * @param y_scale The global Y scale of the engine
 */
void Engine::setGlobalYScale(double y_scale){
    this->target_y_scale = y_scale;
}

/** Returns the global X scale of the engine
 * @return The global X scale of the engine
 */
float Engine::getGlobalXScale(){
    return this->target_x_scale;
}

/** Returns the global Y scale of the engine
 * @return The global Y scale of the engine
 */
float Engine::getGlobalYScale(){
    return this->target_y_scale;
}

/** Goes through all active threads and cleans them up
 */
inline void Engine::threadGC(){
    while(thread_cleanup_queue.load()->empty() == false){
        std::thread::id thread_id = thread_cleanup_queue.load()->front();
        thread_cleanup_queue.load()->pop();
        if(thread_map.find(thread_id) != thread_map.end()){
            std::thread* thread_ptr = thread_map[thread_id];
            if(thread_ptr != nullptr && thread_ptr->joinable()){
                thread_ptr->join();
                thread_map.erase(thread_id);
            }
        }
    }
}

/** Handles the default collision between two objects
 * @param obj1 The first object in the collision
 * @param box1 The first hitbox in the collision
 * @param obj2 The second object in the collision
 * @param box2 The second hitbox in the collision
 */
void Engine::handleDefaultCollision(Object* obj1, Hitbox* box1, Object* obj2, Hitbox* box2){
    unsigned int box1_prop = box1->getType();
    unsigned int box2_prop = box2->getType();

    //If both hotboxes are collision boxes
    if(box1_prop & COLLISION && box2_prop & COLLISION){
        //If the either box is the environment (or has been bumped by the environment)
        if(box1_prop & ENVIRONMENT || box2_prop & ENVIRONMENT || obj1->getEnvBump() || obj2->getEnvBump()){
            Hitbox* mov_box,* env_box;
            Object* mov_obj,* env_obj;
            if(box1_prop & ENVIRONMENT || obj1->getEnvBump()){
                env_box = box1;
                env_obj = obj1;
                mov_box = box2;
                mov_obj = obj2;
            }
            else{
                env_box = box2;
                env_obj = obj2;
                mov_box = box1;
                mov_obj = obj1;
            }

            //If the mov_box is below the env_box
            if(env_box->getZMin() >= mov_box->getZMax() && env_box->getZMin() < mov_obj->getOldZ() + mov_box->getDepth() - mov_box->getZOffset()){
                mov_obj->setZ(env_box->getZMin() - mov_box->getDepth() - mov_box->getZOffset());
                mov_obj->setZVel(0);
            }
            //If the mov_box is above the env_box
            else if(mov_box->getZMax() >= env_box->getZMin() && mov_box->getZMax() <= env_obj->getOldZ() - env_box->getZOffset()){
                mov_obj->setZ(env_box->getZMax() + mov_box->getZOffset());
                mov_obj->setZVel(0);
            }
            
            if(mov_box->checkCollision(env_box) == false){
                return;
            }

            double old_x = mov_obj->getOldX();
            double old_y = mov_obj->getOldY();

            double x = mov_obj->getX();
            double y = mov_obj->getY();

            double x_movement = (old_x - x) / ROLLBACK_STEP;
            double y_movement = (old_y - y) / ROLLBACK_STEP;

            if(x_movement == 0.0 && y_movement == 0.0){
                x_movement = -1.0;
                y_movement = -1.0;
            }

            mov_obj->setEnvBump();

            //Handle Z collision
            //If we're hitting the ceiling
            //If we're hitting the floor

            //Rollback to the point where we're no longer colliding
            for(int i = 0; mov_box->checkCollision(env_box); i++){
                x += x_movement;
                y += y_movement;
                mov_obj->setX(x);
                mov_obj->setY(y);
            }
        }
        //If neither are the environment
        else{
            //Calculating new forces
            float obj1_x_force = obj1->getXVel() * obj1->getMass();
            float obj1_y_force = obj1->getYVel() * obj1->getMass();
            float obj2_x_force = obj2->getXVel() * obj2->getMass();
            float obj2_y_force = obj2->getYVel() * obj2->getMass();

            float mass_sum = obj1->getMass() + obj2->getMass();

            double x_force = (obj1_x_force + obj2_x_force) / mass_sum;
            double y_force = (obj1_y_force + obj2_y_force) / mass_sum;

            obj1->setXVel(x_force);
            obj1->setYVel(y_force);

            obj2->setXVel(x_force);
            obj2->setYVel(y_force);
        }
    }
    else if((box1_prop & ENVIRONMENT && box2_prop & GROUNDING_ZONE) || (box1_prop & GROUNDING_ZONE && box2_prop & ENVIRONMENT)){
        Hitbox* env_box;
        Object* grnd_obj;

        if(box1_prop & GROUNDING_ZONE){
            env_box = box2;
            grnd_obj = obj1;
        }
        else{
            env_box = box1;
            grnd_obj = obj2;
        }

        double env_z = env_box->getZMax();
        if(grnd_obj->getNextGround() < env_z){
            grnd_obj->setNextGround(env_z);
        }
    }
}

/** Make a combined deep copy of the object list (so that things of different zones can interact if they're adjacent)
 * @return A full list of all active objects in the engine
 */
void Engine::buildFullEntityList(){
    if(unlikely(this->entities.obj == nullptr)){
        this->entities.obj = new ObjectList;
        this->entities.obj->next = nullptr;
        this->entities.obj->obj = nullptr;
    }
    ObjectList* obj_iter = this->entities.obj;
    ObjectList* obj_iter_delayed = obj_iter;

    if(unlikely(this->entities.ui == nullptr)){
        this->entities.ui = new UIElementList;
        this->entities.ui->next = nullptr;
        this->entities.ui->element = nullptr;
    }
    UIElementList* ui_iter = this->entities.ui;
    UIElementList* ui_iter_delayed = ui_iter;

    ZoneList* zone_iter = this->active_zones;
    //For each zone
    while(zone_iter != NULL){
        //For each object in the zone
        ObjectList* new_objects = zone_iter->zone->getObjects();
        while(new_objects != NULL && new_objects->obj != NULL){
            //Set the object
            obj_iter->obj = new_objects->obj;

            //If there's not a next object in the current list and no more new objects
            if(obj_iter->next == nullptr){
                obj_iter->next = new ObjectList;
                obj_iter->next->next = nullptr;
                obj_iter->next->obj = nullptr;
            }

            obj_iter_delayed = obj_iter;
            obj_iter = obj_iter->next;
            new_objects = new_objects->next;
        }

        //For each UI element in the zone
        UIElementList* new_elements = zone_iter->zone->getUIElements();
        while(new_elements != NULL && new_elements->element != NULL){
            ui_iter->element = new_elements->element;

            //We have to do this for every entry except for the first
            if(ui_iter->next == nullptr){
                ui_iter->next = new UIElementList;
                ui_iter->next->next = nullptr;
                ui_iter->next->element = nullptr;
            }

            ui_iter_delayed = ui_iter;
            ui_iter = ui_iter->next;
            new_elements = new_elements->next;
        }

        zone_iter = zone_iter->next;
    }

    //Clean up all hanging nodes (this can happen if there are less objects this frame than the last)
    if(likely(obj_iter != this->entities.obj)){
        while(obj_iter != nullptr){
            ObjectList* tmp = obj_iter;
            obj_iter = obj_iter->next;
            delete tmp;
        }
        obj_iter_delayed->next = nullptr;
    }
    else{
        delete obj_iter;
        this->entities.obj = nullptr;
    }

    if(likely(ui_iter != this->entities.ui)){
        while(ui_iter != nullptr){
            UIElementList* tmp = ui_iter;
            ui_iter = ui_iter->next;
            delete tmp;
        }
        ui_iter_delayed->next = nullptr;
    }
    else{
        delete ui_iter;
        this->entities.ui = nullptr;
    }
}

/** Frees the full list of objects generated for each step
 * @param all_objects The full list of objects
 */
void Engine::freeFullEntityList(){
    ObjectList* free_objects;
    ObjectList* all_objects = this->entities.obj;
    while(all_objects != NULL){
        free_objects = all_objects->next;
        delete all_objects;
        all_objects = free_objects;
    }

    UIElementList* free_ui;
    UIElementList* all_ui = this->entities.ui;
    while(all_ui != NULL){
        free_ui = all_ui->next;
        delete all_ui;
        all_ui = free_ui;
    }
}

/** Adds a new thread to the engine (to get cleaned up once the thread's execution halts)
 * @param thread The thread to add
 */
void Engine::addThread(std::thread* thread){
    thread_map[thread->get_id()] = thread;
}

/** Pushes a thread to the thread cleanup queue
 * @param thread_id The thread ID you'd like to clean up
 */
void Engine::cleanupThread(std::thread::id thread_id){
    if(thread_id != base_thread_id){
        thread_cleanup_queue.load(std::memory_order_relaxed)->push(thread_id);
    }
}

/** Adds a new zone to the game
 * @param zone The zone to add
 */
void Engine::addZone(Zone* zone){
    if(getZone(zone->getName()) == nullptr){
        ZoneList* new_zone = new ZoneList;
        new_zone->zone = zone;
        new_zone->next = this->zones;
        zones = new_zone;
    }
    else{
        delete zone;
    }
}

/** Adds an object to the zone
 * @param zone The zone we're adding an object to
 * @param object The object we're adding to the zone
 * @return -1 if the zone isn't found, 0 otherwise
 */
int Engine::addObject(const char* zone, Object* object){
    Zone* zone_ptr = getZone(zone);
    if(zone == nullptr){
        return -1;
    }

    zone_ptr->addObject(object);

    return 0;
}

/** Adds an UI Element to the zone
 * @param zone The zone we're adding a UI element to
 * @param element The UI element we're adding to the zone
 * @return -1 if the zone isn't found, 0 otherwise
 */
int Engine::addUIElement(const char* zone, UIElement* element){
    Zone* zone_ptr = getZone(zone);
    if(zone == nullptr){
        return -1;
    }

    zone_ptr->addUIElement(element);

    return 0;
}

/**Adds a surface to the sprite hash
 * @param key The key representing the surface
 * @param surface The surface being added to the hash
 */
void Engine::addSurface(const char* key, SDL_Surface* surface){
    this->sprite_hash->add(key, surface);
}

/**Adds a sound to the sprite hash
 * @param key The key representing the sound
 * @param sound The sound being added to the hash
 */
void Engine::addSound(const char* key, Sound* sound){
    this->sound_hash->add(key, sound);
}

/**Adds a music to the sprite hash
 * @param key The key representing the music
 * @param music The music being added to the hash
 */
void Engine::addMusic(const char* key, Music* music){
    this->music_hash->add(key, music);
}

/**Adds a font to the sprite hash
 * @param key The key representing the font
 * @param font The font being added to the hash
 */
void Engine::addFont(const char* key, Font* font){
    this->font_hash->add(key, font);
}

/** Moves a Zone to the active_zones ZoneList
 * @param zone_name The name of the zone you wish to move
 */
void Engine::activateZone(const char* zone_name){
    //If it's the first zone
    if(this->zones != nullptr && strcmp(this->zones->zone->getName(), zone_name) == 0){
        ZoneList* moved_zone = this->zones;
        zones = this->zones->next;
        moved_zone->next = this->active_zones;
        this->active_zones = moved_zone;
    }
    else if(this->zones != nullptr){
        ZoneList* zone_lst = this->zones;
        //If it's anything after the first zone
        while(zone_lst->next != NULL){
            if(strcmp(zone_lst->next->zone->getName(), zone_name) == 0){
                ZoneList* moved_zone = zone_lst->next;
                zone_lst->next = moved_zone->next;
                moved_zone->next = this->active_zones;
                this->active_zones = moved_zone;
            }
        }
    }
}

/** Moves a zone from active_zone to zones
 * @param zone_name The name of the zone you wish to deactivate
 */
void Engine::deactivateZone(const char* zone_name){
    //If it's the first zone
    if(zone_name != NULL){
        if(this->zones != nullptr && strcmp(this->active_zones->zone->getName(), zone_name) == 0){
            ZoneList* moved_zone = this->active_zones;
            active_zones = this->active_zones->next;
            moved_zone->next = this->zones;
            this->zones = moved_zone;
        }
        else if(this->zones != nullptr){
            //If it's anything after the first zone
            ZoneList* zone_lst = this->active_zones;
            while(zone_lst->next != NULL){
                if(strcmp(zone_lst->next->zone->getName(), zone_name) == 0){
                    ZoneList* moved_zone = zone_lst->next;
                    zone_lst->next = moved_zone->next;
                    moved_zone->next = this->zones;
                    this->zones = moved_zone;
                }
            }
        }
    }
}

/** Unloads a zone from memory
 * @param zone_name The name of the zone you wish to unload
 */
void Engine::unloadZone(const char* zone_name){
    //If it's the first zone
    if(this->zones != nullptr && strcmp(this->zones->zone->getName(), zone_name) == 0){
        ZoneList* unloaded_zone = this->zones;
        zones = this->zones->next;
        delete unloaded_zone->zone;
        delete unloaded_zone;
    }
    else if(this->zones != nullptr){
        ZoneList* zone_lst = this->zones;
        //If it's anything after the first zone
        while(zone_lst->next != NULL){
            if(strcmp(zone_lst->next->zone->getName(), zone_name) == 0){
                ZoneList* unloaded_zone = zone_lst->next;
                zone_lst->next = unloaded_zone->next;
                delete unloaded_zone->zone;
                delete unloaded_zone;
            }
        }
    }
}

/** Gets all zones
 * @return A ZoneList* of all zones
 */
ZoneList* Engine::getZones(){
    return this->zones;
}

/** Gets a zone by name
 * @param zone_name The name of the zone you want
 * @return The requested zone
 */
Zone* Engine::getZone(const char* zone_name){
    ZoneList* zone_cursor = this->zones;
    while(zone_cursor != nullptr){
        if(strcmp(zone_name, zone_cursor->zone->getName()) == 0){
            return zone_cursor->zone;
        }
        zone_cursor = zone_cursor->next;
    }

    zone_cursor = this->active_zones;
    while(zone_cursor != nullptr){
        if(strcmp(zone_name, zone_cursor->zone->getName()) == 0){
            return zone_cursor->zone;
        }
        zone_cursor = zone_cursor->next;
    }

    return nullptr;
}

/** Gets all active zones
 * @return A ZoneList* of all active zones
 */
ZoneList* Engine::getActiveZones(){
    return this->active_zones;
}

/** Gets the engine's sound board
 * @return The engine's sound board
 */
SoundBoard* Engine::getSoundBoard(){
    return this->sound_board;
}

/** Gets the global gravity val of the engine
 * @return The global gravity val
 */
float Engine::getGravity(){
    return this->gravity;
}

/** Sets the global gravity val of the engine
 * @param gravity The global gravity val
 */
void Engine::setGravity(float gravity){
    this->gravity = gravity;
}

/** Gets a texture from the engine
 * @param key The texture's identifier in the hash table
 * @return A nullptr if not found (& it can't be loaded), a pointer to the SDL_Surface otherwise
 */
SDL_Surface* Engine::getSurface(const char* key){
    return this->sprite_hash->get(key);
}

/** Gets a sound from the engine
 * @param key The sound's identifier in the hash table
 * @return A nullptr if not found (& it can't be loaded), a pointer to the Sound otherwise
 */
Sound* Engine::getSound(const char* key){
    return this->sound_hash->get(key);
}

/** Gets a music from the engine
 * @param key The music's identifier in the hash table
 * @return A nullptr if not found (& it can't be loaded), a pointer to the Music otherwise
 */
Music* Engine::getMusic(const char* key){
    return this->music_hash->get(key);
}

/** Gets a font from the engine
 * @param key The font's identifier in the hash table
 * @return A nullptr if not found (& it can't be loaded), a pointer to the Font otherwise
 */
Font* Engine::getFont(const char* key){
    return this->font_hash->get(key);
}