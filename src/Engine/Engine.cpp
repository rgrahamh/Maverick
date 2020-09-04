#include "./Engine.hpp"

/** Engine's parameterized constructor
 * @param zones The zones that the game engine is initialized with
 */
Engine::Engine(){
    //Initialization of window and camera
	this->window = new sf::RenderWindow(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().width), "SFML Window");
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
}

/** The function that is called to start the game engine's operation
 */
void Engine::start(){
    //Setting up the sprite hash table
    sprite_hash = new SpriteHash(2048);

    //Create initial zone (TAKE OUT LATER!)
    Zone* zone = new Zone("Zone 1");
    Player* player = new Player(0.0f, 0.0f, HUMAN, ATTACKER, new Stats(), new Mastery(), new Abilities(), new Equipment(), NULL, 1, 12);
    player->addSprite(0, "assets/Tails.png", 16, 0, 0);
    player->setScale(0, 0.1, 0.1);
    zone->addObject(player);
    this->addZone(zone);
    this->activateZone(zone->getName());

    //Setting the reference
    camera->setReference(player);

    bool exit_game = 0;
	while(exit_game == 0){
		inputStep();
		physicsStep();
		collisionStep();
		drawStep();
	}
}

void Engine::addZone(Zone* zone){
    ZoneLst* new_zone = new ZoneLst;
    new_zone->zone = zone;
    new_zone->next = this->zones;
    zones = new_zone;
}

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

void Engine::inputStep(){
    return;
}

void Engine::physicsStep(){
    return;
}

void Engine::collisionStep(){
    return;
}

/** The draw step of the game engine
 */
void Engine::drawStep(){
    //Make a combined deep copy of the object list (so that things of different zones can interact if they're adjacent)
    ObjectLst* all_objects = new ObjectLst;
    ObjectLst* obj_iter = all_objects;

    bool first_run = true;
    ZoneLst* zone_iter = this->active_zones;
    while(zone_iter != NULL){
        ObjectLst* new_objects = zone_iter->zone->getObjects();
        while(new_objects->next != NULL){
            if(first_run != true){
                obj_iter->next = new ObjectLst;
                obj_iter = obj_iter->next;
            }
            obj_iter->obj = new_objects->obj;
            new_objects = new_objects->next;

            first_run = false;
        }
        zone_iter = zone_iter->next;
    }

    //Draw operation
    this->camera->_draw(all_objects);

    ObjectLst* free_objects;
    while(all_objects != NULL){
        free_objects = all_objects->next;
        delete all_objects;
        all_objects = free_objects;
    }
}