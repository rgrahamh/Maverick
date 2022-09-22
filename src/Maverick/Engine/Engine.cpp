#include "Engine.hpp"

// EVENTUALLY, we'll want to take out this include and replace the "loadZone()" with a proper zone loading function
#include "Maverick/FileHandler/Saver/Saver.hpp"
#include "Maverick/FileHandler/Loader/Loader.hpp"

std::atomic<bool> exit_game;

bool endian;
bool debug = false;
bool graphics_init = false;

Engine* Engine::engine = nullptr;

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
    }

    if(SDL_GetNumAudioDevices(0) > 0){
        //Play around w/ the audio frequency
        if(Mix_OpenAudio(44100, AUDIO_S32LSB, 2, 512) < 0){
            printf("Failed to init Mixer Audio! ERR: %s\n", Mix_GetError());
        }

        for(int i = 0; i < SDL_GetNumAudioDevices(0); i++){
            printf("Audio device %i: %s\n", i, SDL_GetAudioDeviceName(i, 0));
        }
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
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL){
        printf("Renderer is null; exiting");
        fflush(stdout);
        exit(-1);
    }
    this->camera = new Camera(renderer, window, nullptr, CAMERA_FOLLOW_MODE::FIXED_FOLLOW, 0.08);

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
}

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

void Engine::start(){
    gameLoop();
}

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
                printf("FPS: %lu\n", fps);
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
	}
}

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

        obj_cursor->obj->_action(control);
        obj_cursor = obj_cursor->next;
    }

    UIElementList* ui_cursor = all_entities->ui;
    while(ui_cursor != NULL){
        UIElement* curr_element = ui_cursor->element;
        if(unlikely(curr_element->isActive() == false)){
            ui_cursor = ui_cursor->next;
            continue;
        }

        ui_cursor->element->_action(control);
        ui_cursor = ui_cursor->next;
    }
    this->globalAction();
}

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

void Engine::collisionStep(ObjectList* all_objects){
    //Sister arrays for the list matricies
    ObjectList* object_matrix[16][9] = {{nullptr}};
    HitboxList* hitbox_matrix[16][9] = {{nullptr}};

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
            //Set up the object & hitbox matricies
            //Go by height as the outer loop since it eliminates the most
            if(x_iter > 0 && y_iter > 0){
                int j = 0;
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

inline ObjectList* Engine::drawSort(ObjectList* curr_obj){
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

uint64_t Engine::getState(){
    return this->state;
}

Camera* Engine::getCamera(){
    return this->camera;
}

SDL_Window* Engine::getWindow(){
    return this->window;
}

bool Engine::checkState(uint64_t chk_state){
    return (this->state & chk_state) != 0;
}

void Engine::setState(uint64_t state){
    this->state = state;
}

void Engine::setGlobalXScale(double x_scale){
    this->target_x_scale = x_scale;
}

void Engine::setGlobalYScale(double y_scale){
    this->target_y_scale = y_scale;
}

float Engine::getGlobalXScale(){
    return this->target_x_scale;
}

float Engine::getGlobalYScale(){
    return this->target_y_scale;
}

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

            double old_env_z_min = env_obj->getOldZ() + env_box->getZOffset();
            double old_env_z_max = env_obj->getOldZ() + env_box->getZOffset() + env_box->getDepth();

            double old_mov_z_min = mov_obj->getOldZ() + mov_box->getZOffset();
            double old_mov_z_max = mov_obj->getOldZ() + mov_box->getZOffset() + mov_box->getDepth();

            //If the bottom of the mov_box hit the top of the env_box
            if(mov_box->getZMin() <= env_box->getZMax() && old_mov_z_min >= old_env_z_max){
                mov_obj->setZ(env_box->getZMax() + mov_box->getZOffset());
                mov_obj->setZVel(0);
                return;
            }
            //If the top of the mov_box hit the bottom of the env_box
            else if(mov_box->getZMax() >= env_box->getZMin() && old_mov_z_max <= old_env_z_min){
                mov_obj->setZ(env_box->getZMin() - mov_box->getDepth());
                mov_obj->setZVel(0);
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

int Engine::addObject(const char* zone, Object* object){
    Zone* zone_ptr = getZone(zone);
    if(zone == nullptr){
        return -1;
    }

    zone_ptr->addObject(object);

    return 0;
}

int Engine::addUIElement(const char* zone, UIElement* element){
    Zone* zone_ptr = getZone(zone);
    if(zone == nullptr){
        return -1;
    }

    zone_ptr->addUIElement(element);

    return 0;
}

void Engine::addSurface(const std::string& key, SDL_Surface* surface){
    if(surface != nullptr && this->sprite_hash.find(key) == this->sprite_hash.end()){
        this->sprite_hash[key] = surface;
    }
}

void Engine::addSound(const std::string& key, Sound* sound){
    if(sound != nullptr && this->sound_hash.find(key) == this->sound_hash.end()){
        this->sound_hash[key] = sound;
    }
}

void Engine::addMusic(const std::string& key, Music* music){
    if(music != nullptr && this->music_hash.find(key) == this->music_hash.end()){
        this->music_hash[key] = music;
    }
}

void Engine::addFont(const std::string& key, Font* font){
    if(font != nullptr && this->font_hash.find(key) == this->font_hash.end()){
        this->font_hash[key] = font;
    }
}

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

ZoneList* Engine::getZones(){
    return this->zones;
}

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

ZoneList* Engine::getActiveZones(){
    return this->active_zones;
}

SoundBoard* Engine::getSoundBoard(){
    return this->sound_board;
}

float Engine::getGravity(){
    return this->gravity;
}

void Engine::setGravity(float gravity){
    this->gravity = gravity;
}

SDL_Surface* Engine::getSurface(const std::string& key){
    if(this->sprite_hash.find(key) == this->sprite_hash.end()){
        return nullptr;
    }
    return this->sprite_hash[key];
}

Sound* Engine::getSound(const std::string& key){
    if(this->sound_hash.find(key) == this->sound_hash.end()){
        return nullptr;
    }
    return this->sound_hash[key];
}

Music* Engine::getMusic(const std::string& key){
    if(this->music_hash.find(key) == this->music_hash.end()){
        return nullptr;
    }
    return this->music_hash[key];
}

Font* Engine::getFont(const std::string& key){
    if(this->font_hash.find(key) == this->font_hash.end()){
        return nullptr;
    }
    return this->font_hash[key];
}