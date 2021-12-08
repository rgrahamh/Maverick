#include "./Engine.hpp"

// EVENTUALLY, we'll want to take out this include and replace the "loadZone()" with a proper zone loading function
#include "../Zone/ZoneFactory/ZoneFactory.hpp"

static int SDLCALL event_listener(void* userdata, SDL_Event* event){
    if(event->type == SDL_EventType::SDL_QUIT){
        *(bool*)userdata = true;
    }

    return 1;
}

/** Engine's parameterized constructor
 * @param zones The zones that the game engine is initialized with
 */
Engine::Engine(){
    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING)){
        printf("Failed to init everything!\n");
    }
    if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG){
        printf("Failed to init IMG!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
    }
    if(TTF_Init() < 0){
        printf("Failed to init TTF! ERR: %s\n", TTF_GetError());
    }

    //Initialization of control system
    control = new Control();

    int win_x = 1920;
    int win_y = 1080;

    //Initialization of window and camera
	this->window = SDL_CreateWindow("Cyberena", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, win_x, win_y, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if(window == nullptr){
        printf("Could not create window; exiting");
        fflush(stdout);
        return;
    }

    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    //Get rid of SDL_RENDERER_PRESENTVSYNC if we want to take the frame cap off
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL){
        printf("Renderer is null; exiting");
        fflush(stdout);
        return;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    this->camera = new Camera(renderer, window, NULL);

    //Set up the screenshot blit surface
    this->screen_blit_surface = SDL_CreateRGBSurface(0, win_x, win_y, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    this->screen_blit_texture = nullptr;

    //Setting up the texture hash table
    texture_hash = new TextureHash(2048);

    //Set global scales
    this->global_x_scale = 1.0;
    this->global_y_scale = 1.0;

    this->zones = NULL;
    this->active_zones = NULL;

    //Set to the title screen
    this->state = GAME_STATE::OVERWORLD;

    this->entities.obj = (ObjectList*)calloc(1, sizeof(ObjectList));
    this->entities.ui = (UIElementList*)calloc(1, sizeof(UIElementList));

    this->threads = NULL;

    this->delta = 0;
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
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    freeFullEntityList();
}

/** The function that is called to start the game engine's operation
 */
void Engine::start(){

    //Loading the test zone as the first area
    //this->addThread(new std::thread(loadZone, "Test Zone", this, new_objs));
    loadZone("Test Zone", this, nullptr);

    //Loading the level editor
    //this->addThread(new std::thread(loadZone, "led", this, nullptr));
    loadZone("led", this, nullptr);

    //Loading the test zone as the first area
    //this->addThread(new std::thread(loadZone, "global", this, nullptr));
    loadZone("global", this, nullptr);

    gameLoop();
}

/** The primary game loop
 */
void Engine::gameLoop(){
    bool exit_game = false;
    SDL_AddEventWatch(event_listener, &exit_game);

	while(!exit_game){
        buildFullEntityList();
        delta = SDL_GetTicks();

        //Cleanup threads every second
        if(this->delta % 1000 == 0){
            this->threadCleanup();
        }

        delta = SDL_GetTicks() - last_time;
        last_time = SDL_GetTicks();

        this->actionStep(&this->entities);
        this->physicsStep(this->entities.obj);
        if(this->state != GAME_STATE::PAUSE){
            this->collisionStep(this->entities.obj);
        }

        //Different because we need to adjust the object list for draw order
        this->drawStep(&this->entities);
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
        obj_cursor->obj->action(control);
        obj_cursor = obj_cursor->next;
    }

    UIElementList* ui_cursor = all_entities->ui;
    while(ui_cursor != NULL){
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
void Engine::physicsStep(ObjectList* all_objects){
    if(!this->checkState(GAME_STATE::PAUSE)){
        while(all_objects != NULL){
            all_objects->obj->_process(this->delta);
            all_objects = all_objects->next;
        }
    }
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
                case RECT:{
                        HitRect* rect_hitbox = (HitRect*)curr_hitbox;

                        top_bound = rect_hitbox->getY();
                        bot_bound = rect_hitbox->getY() + rect_hitbox->getHeight();
                        left_bound = rect_hitbox->getX();
                        right_bound = rect_hitbox->getX() + rect_hitbox->getWidth();
                    }
                    break;

                case ELLIPSE:{
                        HitEllipse* ellipse_hitbox = (HitEllipse*)curr_hitbox;

                        top_bound = ellipse_hitbox->getY() - ellipse_hitbox->getYRadius();
                        bot_bound = ellipse_hitbox->getY() + ellipse_hitbox->getYRadius();
                        left_bound = ellipse_hitbox->getX() - ellipse_hitbox->getXRadius();
                        right_bound = ellipse_hitbox->getX() + ellipse_hitbox->getXRadius();
                    }
                    break;

                case CONE:{
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
            int x_iter = win_width / x_block;
            int y_iter = win_height / y_block;
            int j = 0;
            //Set up the object & hitbox matricies
            //Go by height as the outer loop since it eliminates the most
            for(int box_y = 0; box_y < win_height; box_y += y_iter){
                //If the top_bound is below box_y + win_height or if the bot_bound is above box_y, go to next
                if(!(top_bound > box_y + y_iter || bot_bound < box_y)){
                    int i = 0;
                    for(int box_x = 0; box_x < win_width; box_x += x_iter){
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
            while(hitbox_lst != NULL){
                //Iterate over every item after the hitbox_lst
                ObjectList* object_cursor = object_lst->next;
                HitboxList* hitbox_cursor = hitbox_lst->next;
                while(hitbox_cursor != NULL){
                    //Possible place for multi-threading!

                    //If both aren't environment and they collide, and the object isn't the same
                    if((!((hitbox_lst->hitbox->getType() & ENVIRONMENT) && (hitbox_cursor->hitbox->getType() & ENVIRONMENT))) && hitbox_lst->hitbox->checkCollision(hitbox_cursor->hitbox) && object_lst->obj != object_cursor->obj){
                        //We want the default collision handling to go last since it's the harshest (and might inhibit special collision cases)
                        object_lst->obj->onCollide(object_cursor->obj, hitbox_lst->hitbox, hitbox_cursor->hitbox);
                        object_cursor->obj->onCollide(object_lst->obj, hitbox_cursor->hitbox, hitbox_lst->hitbox);
                        handleDefaultCollision(object_cursor->obj, hitbox_cursor->hitbox, object_lst->obj, hitbox_lst->hitbox);
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
 */
void Engine::drawStep(EntityList* all_entities){
    SDL_Renderer* renderer = this->camera->getRenderer();

    SDL_RenderClear(renderer);

    SDL_RenderSetScale(renderer, global_x_scale, global_y_scale);

    //Draw operation
    all_entities->obj = this->drawSort(all_entities->obj);
    this->camera->_draw(all_entities->obj, this->delta);

    //Comment out for production builds (seems to have poor performance implications)
    /*if(SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, this->screen_blit_surface->pixels, this->screen_blit_surface->pitch) == 0){
        this->screen_blit_texture = SDL_CreateTextureFromSurface(renderer, this->screen_blit_surface);
    }*/

    SDL_RenderSetScale(renderer, 1.0, 1.0);

    all_entities->ui = (UIElementList*)this->drawSort((ObjectList*)all_entities->ui);
    this->camera->_draw(all_entities->ui, this->delta);

    SDL_RenderPresent(renderer);

    if(this->screen_blit_texture != nullptr){
        SDL_DestroyTexture(this->screen_blit_texture);
        this->screen_blit_texture = nullptr;
    }
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
            if(curr_obj->obj->getDrawLayer() > next_obj->obj->getDrawLayer() ||
              (curr_obj->obj->getDrawAxis() > next_obj->obj->getDrawAxis() && curr_obj->obj->getDrawLayer() == next_obj->obj->getDrawLayer())){
                //Swap node positions & send curr_obj up the draw chain
                curr_obj->next = next_obj->next;
                next_obj->next = this->drawSort(curr_obj);
                return next_obj;
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
void Engine::setGlobalXScale(float x_scale){
    this->global_x_scale = x_scale;
}

/** Sets the global Y scale of the engine
 * @param y_scale The global Y scale of the engine
 */
void Engine::setGlobalYScale(float y_scale){
    this->global_y_scale = y_scale;
}

/** Returns the global X scale of the engine
 * @return The global X scale of the engine
 */
float Engine::getGlobalXScale(){
    return this->global_x_scale;
}

/** Returns the global Y scale of the engine
 * @return The global Y scale of the engine
 */
float Engine::getGlobalYScale(){
    return this->global_y_scale;
}

/** Goes through all active threads and cleans them up
 */
void Engine::threadCleanup(){
    ThreadList* cursor = this->threads;
    ThreadList* prev_cursor = this->threads;
    while(cursor != NULL){
        if(cursor->thread->joinable()){
            cursor->thread->join();
            delete cursor->thread;
            
            if(cursor == threads){
                cursor = cursor->next;
                delete threads;
                threads = cursor;
            }
            else{
                ThreadList* curr_entry = cursor;
                cursor = cursor->next;
                prev_cursor->next = cursor;
                delete curr_entry;
            }
        }
        else{
            prev_cursor = cursor;
            cursor = cursor->next;
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

            float old_x = mov_obj->getOldX();
            float old_y = mov_obj->getOldY();

            float x = mov_obj->getX();
            float y = mov_obj->getY();

            float x_movement = old_x - x;
            float y_movement = old_y - y;

            mov_obj->setEnvBump();

            float x_iter = x_movement / ROLLBACK_STEP;
            float y_iter = y_movement / ROLLBACK_STEP;

            //Rollback to the point where we're no longer colliding
            for(int i = 0; mov_box->checkCollision(env_box); i++){
                x += x_iter;
                y += y_iter;
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

            float x_force = (obj1_x_force + obj2_x_force) / mass_sum;
            float y_force = (obj1_y_force + obj2_y_force) / mass_sum;

            obj1->setXVel(x_force);
            obj1->setYVel(y_force);

            obj2->setXVel(x_force);
            obj2->setYVel(y_force);
        }
    }
}

/** Make a combined deep copy of the object list (so that things of different zones can interact if they're adjacent)
 * @return A full list of all active objects in the engine
 */
void Engine::buildFullEntityList(){
    ObjectList* obj_iter = this->entities.obj;
    ObjectList* obj_iter_delayed = obj_iter;
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
    if(obj_iter != this->entities.obj){
        while(obj_iter != nullptr){
            ObjectList* tmp = obj_iter;
            obj_iter = obj_iter->next;
            delete tmp;
        }
    }
    obj_iter_delayed->next = nullptr;

    if(ui_iter != this->entities.ui){
        while(ui_iter != nullptr){
            UIElementList* tmp = ui_iter;
            ui_iter = ui_iter->next;
            delete tmp;
        }
    }
    ui_iter_delayed->next = nullptr;

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
    ThreadList* thread_entry = new ThreadList;
    thread_entry->thread = thread;
    thread_entry->next = threads;
    threads = thread_entry;
}

/** Adds a new zone to the game
 * @param zone The zone to add
 */
void Engine::addZone(Zone* zone){
    ZoneList* new_zone = new ZoneList;
    new_zone->zone = zone;
    new_zone->next = this->zones;
    zones = new_zone;
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
