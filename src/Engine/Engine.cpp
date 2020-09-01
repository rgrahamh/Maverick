#include "./Engine.hpp"

/** Engine's parameterized constructor
 * @param zones The zones that the game engine is initialized with
 */
Engine::Engine(ZoneLst* zones){
    //Initialization of window and camera
	this->window = new sf::RenderWindow(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().width), "SFML Window");
    this->camera = new Camera();

    this->zones = zones;

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


Zone* Engine::loadZone(unsigned char zone_num){
	if(zone_num == 1){
		Zone* zone = new Zone();
        Player* player = new Player(0.0f, 0.0f, HUMAN, ATTACKER, new Stats(), new Mastery(), new Abilities(), new Equipment(), new InvSlot(), 1);
    	zone->addObject(player);
	}
	else{
		return NULL;
	}
}

/** The function that is called to start the game engine's operation
 */
void Engine::start(){
    bool exit_game = 1;
	while(exit_game == 0){
		inputStep();
		physicsStep();
		collisionStep();
		drawStep();
	}
}

/** The draw step of the game engine
 */
void Engine::drawStep(){
    //Make a combined deep copy of the object list (so that things of different zones can interact if they're adjacent)
    ObjectLst* all_objects = new ObjectLst;
    ObjectLst* obj_iter = all_objects;

    ZoneLst* zone_iter = this->active_zones;
    while(this->active_zones != NULL){
        ObjectLst* new_objects = active_zones->zone->getObjects();
        while(new_objects != NULL){
            obj_iter->next = new ObjectLst;
            obj_iter = obj_iter->next;
            obj_iter->obj = new_objects->obj;
            new_objects = new_objects->next;
        }
        zone_iter = zone_iter->next;
    }
    obj_iter->next = NULL;

    //Bubble sort by height (since things aren't typically changing order that much between frames). That way we don't have to compare objects outside of its height scope.
    bool done = false;
    while(!done){
        obj_iter = all_objects;
        done = true;
        while(obj_iter != NULL){
            if ((obj_iter->obj->getY() + obj_iter->obj->getHeight()) > (obj_iter->next->obj->getY() + obj_iter->next->obj->getHeight())){
                done = false;
                Object* tmp = obj_iter->next->obj;
                obj_iter->next->obj = obj_iter->obj;
                obj_iter->obj = tmp;
            }
            obj_iter = obj_iter->next;
        }
    }
    camera->_draw(active_zones->zone->getObjects());

    //TODO: DON'T FORGET TO WRITE CODE TO FREE THE MEMORY USED FOR THE NEW ALL_OBJECTS!!!
}