#include "./Engine.hpp"

/** Engine's parameterized constructor
 * @param zones The zones that the game engine is initialized with
 */
Engine::Engine(){
    //Initialization of window and camera
	this->window = new sf::RenderWindow(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().width), "SFML Window");
    window->setFramerateLimit(60);
    this->camera = new Camera(window, NULL);

    this->zones = NULL;
    this->active_zones = NULL;

    //Set to the title screen
    this->state = TITLE;
}

/** Engine's destructor
 */
Engine::~Engine(){
    window->close();
    delete window;
    delete camera;

    ZoneLst* zone_cursor = zones;
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
}

/** The function that is called to start the game engine's operation
 */
void Engine::start(){
    //Setting up the texture hash table
    texture_hash = new TextureHash(2048);

    //Create initial zone (TAKE OUT LATER!)
    Zone* zone = new Zone("Zone 1");

    //Create the player
    Player* player = buildPlayer(0.0f, 0.0f, 0.75, HUMAN, ATTACKER, new Stats(), new Mastery(), new Abilities(), new Equipment(), NULL);
    zone->addObject(player);

    //Create a pillar
    Object* pillar_1 = buildPillar(800.0, 700.0);
    zone->addObject(pillar_1);

    this->addZone(zone);
    this->activateZone(zone->getName());

    //Setting the reference
    camera->setReference(player);

    gameLoop();
}

void Engine::gameLoop(){
	while(this->state != EXIT){
        ObjectLst* all_objects = buildFullObjLst();

		this->actionStep(all_objects);
		this->physicsStep(all_objects);
		this->collisionStep(all_objects);
		this->drawStep(all_objects);

        freeFullObjLst(all_objects);
	}
}

void Engine::actionStep(ObjectLst* all_objects){
    sf::Event event;
    ObjectLst* cursor;
    while(window->pollEvent(event)){
        if(event.type == sf::Event::Closed){
            this->state = EXIT;
            return;
        }
        cursor = all_objects;
        while(cursor != NULL){
            cursor->obj->action(event);
            cursor = cursor->next;
        }
    }
}

void Engine::physicsStep(ObjectLst* all_objects){
    while(all_objects != NULL){
        all_objects->obj->_process();
        all_objects = all_objects->next;
    }
}

void Engine::collisionStep(ObjectLst* all_objects){
    //Sister arrays for the list matricies
    ObjectLst* object_matrix[16][9] = {NULL};
    HitboxLst* hitbox_matrix[16][9] = {NULL};

    //While we're not out of objects
    while(all_objects != NULL){
        Object* curr_object = all_objects->obj;
        HitboxLst* object_hitboxes = all_objects->obj->getHitboxes();
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
                        bot_bound = ellipse_hitbox->getX() + ellipse_hitbox->getXRadius();
                    }
                    break;

                case CONE:{
                        HitCone* cone_hitbox = (HitCone*)curr_hitbox;

                        top_bound = cone_hitbox->getY() - cone_hitbox->getYRadius();
                        bot_bound = cone_hitbox->getY() + cone_hitbox->getYRadius();
                        left_bound = cone_hitbox->getX() - cone_hitbox->getXRadius();
                        bot_bound = cone_hitbox->getX() + cone_hitbox->getXRadius();
                    }
                    break;
            }
            
            int win_width = sf::VideoMode::getDesktopMode().width;
            int win_height = sf::VideoMode::getDesktopMode().height;
            int x_iter = win_width / 16;
            int y_iter = win_height / 9;
            int j = 0;
            //Set up the object & hitbox matricies
            //Go by height as the outer loop since it eliminates the most
            for(int box_y = 0; box_y < win_height; box_y += y_iter){
                //If the top_bound is below box_y + win_height or if the bot_bound is above box_y, go to next
                if(!(top_bound > box_y + win_height || bot_bound < box_y)){
                    int i = 0;
                    for(int box_x = 0; box_x < win_width; box_x += x_iter){
                        //If the left bound is greater than box_x or the right bound is less than box_x, go to next
                        if(!(left_bound > box_x + win_width || right_bound < box_x)){
                            //Insert the object in the list
                            ObjectLst* new_objectlst = new ObjectLst;
                            new_objectlst->next = object_matrix[i][j];
                            new_objectlst->obj = curr_object;
                            object_matrix[i][j] = new_objectlst;

                            //Insert the hitbox in the list
                            HitboxLst* new_hitboxlst = new HitboxLst;
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
            ObjectLst* object_lst = object_matrix[i][j];
            HitboxLst* hitbox_lst = hitbox_matrix[i][j];
            while(hitbox_lst != NULL){
                //Iterate over every item after the hitbox_lst
                ObjectLst* object_cursor = object_lst->next;
                HitboxLst* hitbox_cursor = hitbox_lst->next;
                while(hitbox_cursor != NULL){
                    //If both aren't environment and they collide
                    if((!((hitbox_lst->hitbox->getType() & ENVIRONMENT) && (hitbox_cursor->hitbox->getType() & ENVIRONMENT))) && hitbox_lst->hitbox->checkCollision(hitbox_cursor->hitbox)){
                        object_lst->obj->onCollide(object_cursor->obj, hitbox_lst->hitbox, hitbox_cursor->hitbox);
                        object_cursor->obj->onCollide(object_lst->obj, hitbox_cursor->hitbox, hitbox_lst->hitbox);
                    }
                    object_cursor = object_cursor->next;
                    hitbox_cursor = hitbox_cursor->next;
                }

                //Linked list cleanup
                ObjectLst* tmp_object = object_lst;
                object_lst = object_lst->next;
                delete tmp_object;

                HitboxLst* tmp_hitbox = hitbox_lst;
                hitbox_lst = hitbox_lst->next;
                delete tmp_hitbox;
            }
        }
    }
}

/** The draw step of the game engine
 */
void Engine::drawStep(ObjectLst* all_objects){
    //Draw operation
    this->camera->_draw(all_objects);
}

/** Make a combined deep copy of the object list (so that things of different zones can interact if they're adjacent)
 * @return A full list of all active objects in the engine
 */
ObjectLst* Engine::buildFullObjLst(){
    ObjectLst* all_objects = (ObjectLst*)calloc(sizeof(ObjectLst), 1);
    ObjectLst* obj_iter = all_objects;

    bool first_run = true;
    ZoneLst* zone_iter = this->active_zones;
    while(zone_iter != NULL){
        ObjectLst* new_objects = zone_iter->zone->getObjects();
        while(new_objects != NULL){
            if(first_run != true){
                obj_iter->next = new ObjectLst;
                obj_iter = obj_iter->next;
                obj_iter->next = NULL;
            }
            obj_iter->obj = new_objects->obj;
            new_objects = new_objects->next;

            first_run = false;
        }
        zone_iter = zone_iter->next;
    }
    return all_objects;
}

/** Frees the full list of objects generated for each step
 * @param all_objects The full list of objects
 */
void Engine::freeFullObjLst(ObjectLst* all_objects){
    ObjectLst* free_objects;
    while(all_objects != NULL){
        free_objects = all_objects->next;
        delete all_objects;
        all_objects = free_objects;
    }
}

/** Adds a new zone to the game
 * @param zone The zone to add
 */
void Engine::addZone(Zone* zone){
    ZoneLst* new_zone = new ZoneLst;
    new_zone->zone = zone;
    new_zone->next = this->zones;
    zones = new_zone;
}

/** Moves a Zone to the active_zones ZoneLst
 * @param zone_name The name of the zone you wish to move
 */
void Engine::activateZone(const char* zone_name){
    //If it's the first zone
    if(strcmp(this->zones->zone->getName(), zone_name) == 0){
        ZoneLst* moved_zone = this->zones;
        zones = this->zones->next;
        moved_zone->next = this->active_zones;
        this->active_zones = moved_zone;
    }
    else{
        ZoneLst* zone_lst = this->zones;
        //If it's anything after the first zone
        while(zone_lst->next != NULL){
            if(strcmp(zone_lst->next->zone->getName(), zone_name) == 0){
                ZoneLst* moved_zone = zone_lst->next;
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
        if(strcmp(this->active_zones->zone->getName(), zone_name) == 0){
            ZoneLst* moved_zone = this->active_zones;
            active_zones = this->active_zones->next;
            moved_zone->next = this->zones;
            this->zones = moved_zone;
        }
        else{
            //If it's anything after the first zone
            ZoneLst* zone_lst = this->active_zones;
            while(zone_lst->next != NULL){
                if(strcmp(zone_lst->next->zone->getName(), zone_name) == 0){
                    ZoneLst* moved_zone = zone_lst->next;
                    zone_lst->next = moved_zone->next;
                    moved_zone->next = this->zones;
                    this->zones = moved_zone;
                }
            }
        }
    }
}
