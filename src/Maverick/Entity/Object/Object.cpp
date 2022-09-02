#include "./Object.hpp"
#include "../../Zone/Zone.hpp"
#include "../../Engine/Engine.hpp"

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
      : Entity(name, layer){
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

    this->hitbox_immunity = nullptr;

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

/** Gets the X value of the entity
 * @return The X value of the entity
 */
double Object::getX(){
    return this->x;
}

/** Gets the Y value of the entity
 * @return The Y value of the entity
 */
double Object::getY(){
    return this->y;
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

/** Sets the X posision
 * @param x The X coordinate
 */
void Object::setX(double x){
    this->x = x;
}

/** Sets the Y posision
 * @param y The Y coordinate
 */
void Object::setY(double y){
    this->y = y;
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

    //Updating Y values
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
        this->zA -= Engine::getInstance()->getGravity() * (float)steps;
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

/** Adds a hitbox to a given animation on either the spepcified sprite of an animation or the last-added sprite of the animation (if -1)
 * @param animation_name The animation name
 * @param shape The hitbox shape
 * @param x_offset The X offset of the hitbox
 * @param y_offset The Y offset of the hitbox
 * @param x_element The X width/radius of the hitbox
 * @param y_element The Y width/radius of the hitbox
 * @param type The type flags of the hitbox
 * @param sprite_num The sprite number that is being used
 * @param immunity_timer The hitbox immunity timer
 * @return 0 on success, -1 if the animation doesn't exist
 */
int Object::addHitbox(const char* animation_name, HITBOX_SHAPE shape, double x_offset, double y_offset, double z_offset, double x_element,
                       double y_element, double depth, unsigned int type, int sprite_num, int hitbox_group, uint32_t immunity_timer){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }

    Hitbox* hitbox;
    //We should never hit the HIT_CONE in this case.
    if(shape == HIT_RECT){
        hitbox = (Hitbox*)new HitRect(&(this->x), &(this->y), &(this->z), x_offset, y_offset, z_offset, x_element, y_element, depth, type, hitbox_group, immunity_timer);
    }
    else{
        hitbox = (Hitbox*)new HitEllipse(&(this->x), &(this->y), &(this->z), x_offset, y_offset, z_offset, x_element, y_element, depth, type, hitbox_group, immunity_timer);
    }

    animation->addHitbox(hitbox, sprite_num);
    return 0;
}

/** Adds an immunity to the hitbox of another entity
 * @param other The other entity (that owns the passed-in hitbox; needed for hitbox group checks)
 * @param hitbox The hitbox to add immunity for
 */
void Object::addHitboxImmunity(Entity* other, Hitbox* hitbox){
    //If the disable timer is 0, there will be no immunity added
    uint32_t ignore_timer = hitbox->getImmunityTimer();
    if(ignore_timer == 0){
        return;
    }

    HitboxImmunityList* new_immunity = new HitboxImmunityList;
    new_immunity->entity = other;
    new_immunity->hitbox = hitbox;
    new_immunity->hitbox_group = hitbox->getHitboxGroup();
    new_immunity->ignore_timer = ignore_timer;

    if(this->hitbox_immunity != nullptr){
        new_immunity->next = this->hitbox_immunity;
    }
    else{
        new_immunity->next = nullptr;
    }
    this->hitbox_immunity = new_immunity;
}

/** Checks immunity against the hitbox of another entity
 * @param other The other entity (that owns the passed-in hitbox; needed for hitbox group checks)
 * @param hitbox The hitbox to check immunity for
 */
bool Object::checkHitboxImmunity(Entity* other, Hitbox* hitbox){
    if(this->hitbox_immunity == nullptr){
        return false;
    }

    HitboxImmunityList* cursor = this->hitbox_immunity;
    uint32_t hitbox_group = cursor->hitbox_group;
    while(cursor != nullptr){
        if(cursor->hitbox == hitbox || (other == cursor->entity && cursor->hitbox_group == hitbox_group)){
            return true;
        }

        cursor = cursor->next;
    }

    return false;
}

/** Cleans up the hitbox immunity list; should be run in the _process step
 * @param delta The amount of time (in ms) that have passed since the last frame
 */
void Object::cleanupHitboxImmunity(uint64_t delta){
    HitboxImmunityList* cursor = this->hitbox_immunity;
    HitboxImmunityList* last_cursor = this->hitbox_immunity;
    while(cursor != nullptr){
        if(cursor->ignore_timer <= delta){
            HitboxImmunityList* tmp = cursor;

            cursor = cursor->next;
            if(tmp == this->hitbox_immunity){
                this->hitbox_immunity = cursor;
                last_cursor = cursor;
            }
            else{
                last_cursor->next = cursor;
            }

            delete tmp;
        }
        else{
            cursor->ignore_timer -= delta;
            last_cursor = cursor;
            cursor = cursor->next;
        }
    }
}

/** Gets the hitboxes of the current animation state
 * @return The HitboxList containing the entity's current hitboxes
 */
HitboxList* Object::getHitboxes(){
    if(this->active_animation == nullptr){
        return nullptr;
    }
    return this->active_animation->getHitboxes();
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
 * @return -1 on failure, 0 on success
 */
int Object::serializeData(FILE* file, Zone* base_zone){
    if(Entity::serializeData(file, base_zone) == -1){
        return 0;
    }

    //Write X/Y offsets from the zone origin
    double write_x = this->x - base_zone->getGlobalX();
    double write_y = this->y - base_zone->getGlobalY();

    WriteVar((uint64_t)write_x, uint64_t, file);
    WriteVar((uint64_t)write_y, uint64_t, file);

    return 0;
}

/** Serializing object assets (to be overridden by children, as necessary)
 * @param file An open file to write to
 * @param serialize_set The serialization set (logs saved assets)
 * @return -1 on failure, 0 on success
 */
int Object::serializeAssets(FILE* file, SerializeSet& serialize_set){
    return Entity::serializeAssets(file, serialize_set);
}