#include "./Object.hpp"

/** Object parameterized constructor
 * @param start_x The starting X location of the object
 * @param start_y The starting Y location of the objcet
 * @param friction The object's coefficient of friction
 * @param draw_layer The draw layer of the object
 * @param animation_num The number of animations
 */
Object::Object(const char* name, float start_x, float start_y, float friction, float mass, unsigned int animation_num, int draw_layer)
      : Entity(name, start_x, start_y, animation_num, draw_layer){
    int name_len = strlen(name);
    this->name = (char*)malloc(name_len + 1);
    memcpy(this->name, name, name_len);
    this->name[name_len] = '\0';
    
    //Initializing position, velocity, and acceleration
    this->x = start_x;
    this->y = start_y;
    this->old_x = start_x;
    this->old_y = start_y;
    this->xV = 0;
    this->yV = 0;
    this->xA = 0;
    this->yA = 0;

    //Initializing various physics elements
    this->friction = friction;
    this->mass = mass;

    //Initializing animation/visibility attributes
    this->active = true;
    this->visible = true;
    this->active_animation = 0;
    this->animation_num = animation_num;
    this->animations = (Animation**)calloc(sizeof(Animation*), animation_num);
    for(unsigned int i = 0; i < animation_num; i++){
        animations[i] = new Animation(&(this->x), &(this->y), draw_layer);
    }

    //Initializing attributes
    this->attr = new HashTable(64);
}

/** Destructor for objects
 */
Object::~Object(){
}

/** Gets the old X value of the object
 * @return The old X value of the object
 */
float Object::getOldX(){
    return this->old_x;
}

/** Gets the old Y value of the object
 * @return The old Y value of the object
 */
float Object::getOldY(){
    return this->old_y;
}

/** Gets the X velocity of the object
 * @return The X velocity of the object
 */
float Object::getXVel(){
    return this->xV;
}

/** Gets the Y velocity of the object
 * @return The Y velocity of the object
 */
float Object::getYVel(){
    return this->yV;
}

/** Gets the mass of the object (in lbs)
 */
float Object::getMass(){
    return this->mass;
}

/** Gets if the object has bumped against the environment (or an entity which has bumped against the environment)
 * @return If the object has bumped against the environemnt
 */
bool Object::getEnvBump(){
    return this->env_bump;
}

/** Sets the X velocity
 * @param xV The X velocity
 */
void Object::setXVel(float xV){
    this->xV = xV;
}

/** Sets the Y velocity
 * @param yV The Y velocity
 */
void Object::setYVel(float yV){
    this->yV = yV;
}

/** Sets the environmental bump to true
 */
void Object::setEnvBump(){
    this->env_bump = true;
}

/** Applies force to an object
 * @param xA The X newtons of the force
 * @param yA The Y newtons of the force
 */
void Object::applyForce(float xA, float yA){
    this->xA += xA / this->mass;
    this->yA += yA;
}

/** Called during input step; calls action function
 * @param event The event being interpreted
 */
void Object::_action(Control* control){
    this->action(control);
}

/** Calculates any actions taken; should be overridden by children if used
 * @param event The event being interpreted
 */
void Object::action(Control* control){
    return;
}

/** Called during the process step; performs object processing calculations
 */
void Object::_process(uint32_t delta){
    this->process(delta);

    //Updating old X & Y values
    this->old_x = this->x;
    this->old_y = this->y;

    //Updating environmental bump
    this->env_bump = false;

    //Updating X values
    this->xV = this->xA * delta + (this->xV * (1 - this->friction));
    if(this->xV < 0.1 && this->xV > -0.1){
        this->xV = 0;
    }
    this->x += this->xV;
    this->xA = 0;

    //Updating Y values
    this->yV = this->yA * delta + (this->yV * (1 - this->friction));
    if(this->yV < 0.1 && this->yV > -0.1){
        this->yV = 0;
    }
    this->y += this->yV;
    this->yA = 0;
}

/** Called during the process step by _process; space for users to override with custom processing logics
 */
void Object::process(uint32_t delta){
}

/** Called during the draw step
 */
void Object::_draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y){
    this->draw(renderer, delta, camera_x, camera_y);
}

/** Called during the draw step
 * @param window The window that content is being drawn to
 */
void Object::draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y){
    this->animations[active_animation]->draw(renderer, delta, camera_x, camera_y);
}

/** Called on object collision; should be overridden by children if you want collision logic.
 * @param other The other object
 * @param this_hitbox The hitbox that collided from this object
 * @param other_hitbox The hitbox that collided from the other object
 */
void Object::onCollide(Object* other, Hitbox* this_hitbox, Hitbox* other_hitbox){
	printf("Collided!\n");
}