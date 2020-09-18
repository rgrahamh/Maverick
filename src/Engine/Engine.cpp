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
    Player* player = buildPlayer(0.0f, 0.0f, 0.75, HUMAN, ATTACKER, new Stats(), new Mastery(), new Abilities(), new Equipment(), NULL, 1);
    zone->addObject(player);

    //Create a pillar
    Object* pillar_1 = buildPillar(800.0, 700.0, 1);
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
    ObjectLst* cursor = all_objects;
    while(cursor != NULL){
        cursor->obj->_process();
        cursor = cursor->next;
    }
}

void Engine::collisionStep(ObjectLst* all_objects){
    return;
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
        free(all_objects);
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
void Engine::activateZone(char* zone_name){
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
void Engine::deactivateZone(char* zone_name){
    //If it's the first zone
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
