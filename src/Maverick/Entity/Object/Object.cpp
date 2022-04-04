#include "./Object.hpp"
#include "../../Zone/Zone.hpp"
#include "../../Engine/Engine.hpp"
extern Engine* engine;

/** Object parameterized constructor
 * @param name The name of the object
 * @param start_x The starting X location of the object
 * @param start_y The starting Y location of the object
 * @param friction The object's coefficient of friction
 * @param mass The object's mass
 * @param terminal_velocity The object's terminal velocity
 * @param gravity If the object should have gravity applied to it
 * @param layer The default layer of the object
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
    this->gravity = gravity;

    this->collision_layer = layer;

    this->ground = 0;
    this->next_ground = this->ground;
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

/** Gets the old Z value of the object
 * @return The old Z value of the object
 */
double Object::getOldZ(){
    return this->old_z;
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

/** Gets the ground Z position
 * @return The ground Z position
 */
double Object::getGround(){
    return this->ground;
}

/** Gets the next ground Z position
 * @return The next ground Z position
 */
double Object::getNextGround(){
    return this->next_ground;
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

/** Sets the ground Z position
 * @param ground The ground Z position
 */
void Object::setGround(double ground){
    this->ground = ground;
}

/** Sets the next ground Z position
 * @param next_ground The next ground Z position
 */
void Object::setNextGround(double next_ground){
    this->next_ground = next_ground;
}

/** Applies force to an object
 * @param xA The X newtons of the force
 * @param yA The Y newtons of the force
 */
void Object::applyForce(double xA, double yA, double zA){
    this->xA += xA / this->mass;
    this->yA += yA / this->mass;
    this->zA += zA / this->mass;
}

/** Called during input step; calls action function
 * @param event The event being interpreted
 */
void Object::_action(Control* control){
    this->action(control);
}

/** Calculates any actions taken; should be overridden by children if used
 * @param control Contains the engine's Control entity
 */
void Object::action(Control* control){
    return;
}

/** Called during the process step; performs object processing calculations
 * @param delta The time that has passed since the last process() call (in ms)
 * @param step_size The step size that should be applied
 */
void Object::_process(uint64_t delta, unsigned int steps){
    //Updating old X & Y values
    if(abs(this->old_x - this->x) > 0.1){
        this->old_x = this->x;
    }
    if(abs(this->old_y - this->y) > 0.1){
        this->old_y = this->y;
    }
    if(abs(this->old_z - this->z) > 0.1){
        this->old_z = this->z;
    }
    this->ground = this->next_ground;
    this->next_ground = 0;

    this->process(delta, steps);

    //Updating environmental bump
    this->env_bump = false;

    //Updating X values (CHANGE THESE TO ALTER VEL BY DELTA LATER)
    this->xV += this->xA;
    if(this->xV != 0){
        xV -= this->xV * friction;
        if(this->xV < 0.01 && this->xV > -0.01){
            this->xV = 0;
        }
        else{
            double terminal_velocity_step = (double)this->terminal_velocity * steps;
            if(this->xV > terminal_velocity_step){
                this->xV = terminal_velocity_step;
            }
            else if(this->xV < terminal_velocity_step * -1.0){
                this->xV = terminal_velocity_step * -1.0;
            }
        }
        this->x += this->xV * steps;
    }
    this->xA = 0;

    //Updating Y values (CHANGE THESE TO ALTER VEL BY DELTA LATER)
    this->yV += this->yA;
    if(this->yV != 0){
        this->yV -= this->yV * friction;
        if(this->yV < 0.01 && this->yV > -0.01){
            this->yV = 0;
        }
        else{
            double terminal_velocity_step = (double)this->terminal_velocity * steps;
            if(this->yV > terminal_velocity_step){
                this->yV = terminal_velocity_step;
            }
            else if(this->yV < terminal_velocity_step * -1.0){
                this->yV = terminal_velocity_step * -1.0;
            }
        }
        this->y += this->yV * steps;
    }
    this->yA = 0;

    //Updating Z values
    if(this->z != this->ground && this->gravity){
        this->zA -= engine->getGravity() * steps;
    }

    this->zV += this->zA;
    if(this->zV != 0){
        this->z += this->zV * steps;
        if(this->z <= ground){
            this->z = ground;
            this->zV = 0;
        }
    }
    this->zA = 0;

    cleanupHitboxImmunity(delta);
}

/** Called during the process step by _process; space for users to override with custom processing logics
 * @param delta The time that has passed since the last process() call (in ms)
 * @param step_size The step size that should be applied
 */
void Object::process(uint64_t delta, unsigned int steps){
}

/** Called during the draw step
 * @param renderer The object renderer
 * @param delta The delta in ms since the last draw() operation
 * @param camera_x The left-hand side of the screen's X coordinate
 * @param camera_x The top side of hte screen's Y coordinate
 */
void Object::_draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y){
    this->draw(renderer, delta, camera_x, camera_y);
}

/** Called during the draw step
 * @param renderer The object renderer
 * @param delta The delta in ms since the last draw() operation
 * @param camera_x The left-hand side of the screen's X coordinate
 * @param camera_x The top side of hte screen's Y coordinate
 */
void Object::draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y){
    if(this->active_animation != nullptr){
        this->active_animation->draw(renderer, delta, camera_x, camera_y, this->z);
    }
}

/** Called on object collision; should be overridden by children if you want collision logic.
 * @param other The other object
 * @param this_hitbox The hitbox that collided from this object
 * @param other_hitbox The hitbox that collided from the other object
 */
void Object::onCollide(Object* other, Hitbox* this_hitbox, Hitbox* other_hitbox){
}

/** Serializing object data (WIP)
 * @param file An open file to write to
 * @param base_zone The zone this object belongs to (used for zone-based offsets)
 * @return 0 on success
 */
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

/** Serializing object assets (to be overridden by children, as necessary)
 * @param file An open file to write to
 * @param serialize_set The serialization set (logs saved assets)
 */
int Object::serializeExtendedAssets(FILE* file, SerializeSet& serialize_set){
    return 0;
}