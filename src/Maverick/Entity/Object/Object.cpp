#include "./Object.hpp"
#include "../../Zone/Zone.hpp"

/** Object parameterized constructor
 * @param start_x The starting X location of the object
 * @param start_y The starting Y location of the objcet
 * @param friction The object's coefficient of friction
 * @param draw_layer The draw layer of the object
 * @param animation_num The number of animations
 */
Object::Object(const char* name, float start_x, float start_y, float start_z, float friction, float mass, float terminal_velocity, bool gravity, int layer)
      : Entity(name, start_x, start_y, layer){
    this->type = OBJECT_TYPE::GENERIC_OBJECT;

    //Initializing position, velocity, and acceleration
    this->x = start_x;
    this->y = start_y;
    this->z = start_z;
    this->old_x = start_x;
    this->old_y = start_y;
    this->old_z = start_z;
    this->xV = 0;
    this->yV = 0;
    this->zV = 0;
    this->xA = 0;
    this->yA = 0;
    this->zA = 0;

    //Initializing various physics elements
    this->friction = friction;
    this->mass = mass;
    this->terminal_velocity = terminal_velocity;

    this->collision_layer = layer;

    this->ground_z = 0;
}

/** Destructor for objects
 */
Object::~Object(){
}

/** Gets the old X value of the object
 * @return The old X value of the object
 */
double Object::getOldX(){
    return this->old_x;
}

/** Gets the old Y value of the object
 * @return The old Y value of the object
 */
double Object::getOldY(){
    return this->old_y;
}

/** Gets the old Z value of the object
 * @return The old Z value of the object
 */
double Object::getOldZ(){
    return this->old_z;
}

/** Gets the X velocity of the object
 * @return The X velocity of the object
 */
double Object::getXVel(){
    return this->xV;
}

/** Gets the Y velocity of the object
 * @return The Y velocity of the object
 */
double Object::getYVel(){
    return this->yV;
}

/** Gets the Z velocity of the object
 * @return The Z velocity of the object
 */
double Object::getZVel(){
    return this->zV;
}

/** Gets the Z value of the object
 * @return The Z value of the object
 */
double Object::getZ(){
    return this->z;
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

/** Gets the current object's current sprite
 * @return The object's current sprite
 */
Sprite* Object::getSprite(){
    if(this->active_animation != nullptr){
        return this->active_animation->getSprite();
    }
    else{
        return nullptr;
    }
}

/** Gets the collision layer
 * @return collision_layer The object's collision layer
 */
int Object::getCollisionLayer(){
    return this->collision_layer;
}

/** Gets the terminal velocity
 * @return The terminal velocity
 */
float Object::getTerminalVelocity(){
    return this->terminal_velocity;
}

/** Sets the X velocity
 * @param xV The X velocity
 */
void Object::setXVel(double xV){
    this->xV = xV;
}

/** Sets the Y velocity
 * @param yV The Y velocity
 */
void Object::setYVel(double yV){
    this->yV = yV;
}

/** Sets the Z velocity
 * @param zV The Z velocity
 */
void Object::setZVel(double zV){
    this->zV = zV;
}

/** Sets the Z position
 * @param zV The Z position
 */
void Object::setZ(double z){
    this->z = z;
}

/** Sets the friction
 * @param friction The object's friction
 */
void Object::setFriction(float friction){
    this->friction = friction;
}

/** Sets the environmental bump to true
 */
void Object::setEnvBump(){
    this->env_bump = true;
}

/** Sets the collision layer
 * @param collision_layer The object's new collision layer
 */
void Object::setCollisionLayer(int collision_layer){
    this->collision_layer = collision_layer;
}

/** Applies force to an object
 * @param xA The X newtons of the force
 * @param yA The Y newtons of the force
 */
void Object::applyForce(double xA, double yA){
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
void Object::_process(uint64_t delta){
    this->process(delta);

    //Updating old X & Y values
    if(fabs(this->x - this->old_x) > 1.0 || fabs(this->y - this->old_y) > 1.0){
        this->old_x = this->x;
        this->old_y = this->y;
    }

    if(fabs(this->z - this->old_z) > 1.0){
        this->old_z = this->z;
    }

    //Updating environmental bump
    this->env_bump = false;

    //Updating X values (CHANGE THESE TO ALTER VEL BY DELTA LATER)
    this->xV += this->xA;
    this->xV -= this->friction * ((float)delta / 10.0) * this->xV;
    if(this->xV < 0.1 && this->xV > -0.1){
        this->xV = 0;
    }
    if(abs(this->xV) > this->terminal_velocity){
        this->xV = this->terminal_velocity;
    }
    this->x += this->xV * delta;
    this->xA = 0;

    //Updating Y values (CHANGE THESE TO ALTER VEL BY DELTA LATER)
    this->yV += this->yA;
    this->yV -= this->friction * ((float)delta / 10.0) * this->yV;
    if(this->yV < 0.1 && this->yV > -0.1){
        this->yV = 0;
    }
    this->y += this->yV * delta;
    this->yA = 0;

    //Updating Z values
    if(this->gravity){
        this->zA -= 2 * delta;
    }
    this->zV += this->zA;
    this->z += this->zV * delta;
    if(this->z <= ground_z){
        this->z = ground_z;
    }
    this->zA = 0;

    cleanupHitboxImmunity(delta);
}

/** Called during the process step by _process; space for users to override with custom processing logics
 */
void Object::process(uint64_t delta){
}

/** Called during the draw step
 */
void Object::_draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y){
    this->draw(renderer, delta, camera_x, camera_y);
}

/** Called during the draw step
 * @param window The window that content is being drawn to
 */
void Object::draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y){
    if(this->active_animation != nullptr){
        this->active_animation->draw(renderer, delta, camera_x, camera_y);
    }
}

/** Called on object collision; should be overridden by children if you want collision logic.
 * @param other The other object
 * @param this_hitbox The hitbox that collided from this object
 * @param other_hitbox The hitbox that collided from the other object
 */
void Object::onCollide(Object* other, Hitbox* this_hitbox, Hitbox* other_hitbox){
	printf("Collided!\n");
}

int Object::serializeExtendedData(FILE* file, Zone* base_zone){
    //Write X/Y offsets from the zone origin
    double write_x = this->x - base_zone->getGlobalX();
    double write_y = this->y - base_zone->getGlobalY();

    uint64_t write_x_swap = EndianSwap((uint64_t*)&write_x);
    uint64_t write_y_swap = EndianSwap((uint64_t*)&write_y);
    fwrite(&write_x_swap, sizeof(write_x_swap), 1, file);
    fwrite(&write_y_swap, sizeof(write_y_swap), 1, file);

    return 0;
}

int Object::serializeExtendedAssets(FILE* file, std::unordered_set<std::string>& sprite_set, std::unordered_set<std::string>& audio_set, std::unordered_set<std::string>& music_set){
    return 0;
}