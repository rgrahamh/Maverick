#include "./Object.hpp"

/** Object parameterized constructor
 * @param start_x The starting X location of the object
 * @param start_y The starting Y location of the objcet
 * @param friction The object's coefficient of friction
 * @param draw_layer The draw layer of the object
 * @param animation_num The number of animations
 */
Object::Object(float start_x, float start_y, float friction, float mass, unsigned int animation_num, bool animated){
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
        animations[i] = new Animation(&(this->x), &(this->y), animated);
    }
}

/** Destructor for objects
 */
Object::~Object(){
    for(unsigned int i = 0; i < animation_num; i++){
        delete animations[i];
    }
    free(animations);
}

/** Gets the X value of the object
 * @return The X value of the object
 */
float Object::getX(){
    return this->x;
}

/** Gets the Y value of the object
 * @return The Y value of the object
 */
float Object::getY(){
    return this->y;
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

/** Gets the width of the object
 * @return The width of the object
 */
float Object::getWidth(){
    return this->animations[active_animation]->getSprite()->rect->w;
}

/** Gets the width of the object
 * @return The width of the object
 */
float Object::getHeight(){
    return this->animations[active_animation]->getSprite()->rect->h;
}

/** Gets the mass of the object (in lbs)
 */
float Object::getMass(){
    return this->mass;
}

/** Gets the draw layer of the object
 * @return The draw layer of the object
 */
unsigned int Object::getDrawLayer(){
    return this->animations[active_animation]->getDrawLayer();
}

/** Gets the draw axis of the object
 * @return The draw axis of the object
 */
float Object::getDrawAxis(){
    return this->animations[active_animation]->getDrawAxis();
}

/** Gets the hitboxes of the current animation state
 * @return The HitboxLst containing the object's current hitboxes
 */
HitboxLst* Object::getHitboxes(){
    return this->animations[active_animation]->getHitboxes();
}

/** Sets if the current object is active (can be interacted with)
 * @return If the object is active
 */
bool Object::isActive(){
    return this->active;
}

/** Sets if the current object is visible (will/won't be drawn)
 * @return If the object is active
 */
bool Object::isVisible(){
    return this->visible;
}

/** Gets if the object has bumped against the environment (or an entity which has bumped against the environment)
 * @return If the object has bumped against the environemnt
 */
bool Object::getEnvBump(){
    return this->env_bump;
}

/** Adds a sprite to a given animation
 * @param animation_num The animation number
 * @param sprite_path The filepath to the sprite you're adding
 * @param keyframe The number of frames until the animation progresses
 * @param x_offset The X offset of the sprite
 * @param y_offset The Y offset of the sprite
 */ 
void Object::addSprite(unsigned int animation_num, const char* sprite_path, unsigned int keyframe, float x_offset, float y_offset){
    this->animations[animation_num]->addFrame(sprite_path, keyframe, x_offset, y_offset);
}

/** Adds a hitbox to a given animation on either the spepcified sprite of an animation or the last-added sprite of the animation (if -1)
 * @param animation_num The animation number
 * @param shape The hitbox shape
 * @param x_offset The X offset of the hitbox
 * @param y_offset The Y offset of the hitbox
 * @param x_element The X width/radius of the hitbox
 * @param y_element The Y width/radius of the hitbox
 * @param type The type flags of the hitbox
 * @param sprite_num The sprite number that is being used
 */
void Object::addHitbox(unsigned int animation_num, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type, int sprite_num){
    Hitbox* hitbox;
    //We should never hit the CONE in this case.
    if(shape == RECT){
        hitbox = (Hitbox*)new HitRect(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, type);
    }
    else{
        hitbox = (Hitbox*)new HitEllipse(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, type);
    }

    this->animations[animation_num]->addHitbox(hitbox, sprite_num);
}

/** Adds a hitbox to a given animation on all sprites
 * @param animation_num The animation number
 * @param shape The hitbox shape
 * @param x_offset The X offset of the hitbox
 * @param y_offset The Y offset of the hitbox
 * @param x_element The X width/radius of the hitbox
 * @param y_element The Y width/radius of the hitbox
 * @param type The type flags of the hitbox
 */
void Object::addHitbox(unsigned int animation_num, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type){
    Hitbox* hitbox;
    //We should never hit the CONE in this case.
    if(shape == RECT){
        hitbox = (Hitbox*)new HitRect(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, type);
    }
    else{
        hitbox = (Hitbox*)new HitEllipse(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, type);
    }

    this->animations[animation_num]->addHitbox(hitbox);
}

/** Adds a hitbox to a range of animations on all sprites
 * @param animation_start The starting animation number (inclusive; this index will be added to)
 * @param animation_end The ending animation number (inclusive; this index will be added to)
 * @param shape The hitbox shape
 * @param x_offset The X offset of the hitbox
 * @param y_offset The Y offset of the hitbox
 * @param x_element The X width/radius of the hitbox
 * @param y_element The Y width/radius of the hitbox
 * @param type The type flags of the hitbox
 */
void Object::addHitbox(unsigned int animation_start, unsigned int animation_end, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type){
    Hitbox* hitbox;
    //We should never hit the CONE in this case.
    if(shape == RECT){
        hitbox = (Hitbox*)new HitRect(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, type);
    }
    else{
        hitbox = (Hitbox*)new HitEllipse(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, type);
    }

    for(unsigned int i = animation_start; i <= animation_end && i < animation_num; i++){
        this->animations[i]->addHitbox(hitbox);
    }
}

/** Adds a hitbox to a given animation (including angles and ratios) on either the spepcified sprite of an animation or the last-added sprite of the animation (if -1). Due to the fact cones are the only ones using angles and ratios, this is currently just cones.
 * @param animation_num The animation number
 * @param type The hitbox type
 * @param x_offset The X offset of the hitbox
 * @param y_offset The Y offset of the hitbox
 * @param x_element The X width/radius of the hitbox
 * @param y_element The Y width/radius of the hitbox
 * @param type The type flags of the hitbox
 * @param angle The slice angle
 * @param slice_prop The slice proportion for the hitbox
 * @param sprite_num The sprite number that is being used
 */
void Object::addHitbox(unsigned int animation_num, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type, float angle, float slice_prop, int sprite_num){
    Hitbox* hitbox = (Hitbox*)new HitCone(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, angle, slice_prop, type);
    this->animations[animation_num]->addHitbox(hitbox, sprite_num);
}

/** Adds a hitbox to a given animation (including angles and ratios) on all sprites. Due to the fact cones are the only ones using angles and ratios, this is currently just cones.
 * @param animation_num The animation number
 * @param type The hitbox type
 * @param x_offset The X offset of the hitbox
 * @param y_offset The Y offset of the hitbox
 * @param x_element The X width/radius of the hitbox
 * @param y_element The Y width/radius of the hitbox
 * @param type The type flags of the hitbox
 * @param angle The slice angle
 * @param slice_prop The slice proportion for the hitbox
 */
void Object::addHitbox(unsigned int animation_num, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type, float angle, float slice_prop){
    Hitbox* hitbox = (Hitbox*)new HitCone(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, angle, slice_prop, type);
    this->animations[animation_num]->addHitbox(hitbox);
}

/** Adds a hitbox to a given animation (including angles and ratios) on all sprites. Due to the fact cones are the only ones using angles and ratios, this is currently just cones.
 * @param animation_start The starting animation number (inclusive; this index will be added to)
 * @param animation_end The ending animation number (inclusive; this index will be added to)
 * @param type The hitbox type
 * @param x_offset The X offset of the hitbox
 * @param y_offset The Y offset of the hitbox
 * @param x_element The X width/radius of the hitbox
 * @param y_element The Y width/radius of the hitbox
 * @param type The type flags of the hitbox
 * @param angle The slice angle
 * @param slice_prop The slice proportion for the hitbox
 */
void Object::addHitbox(unsigned int animation_start, unsigned int animation_end, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type, float angle, float slice_prop){
    Hitbox* hitbox = (Hitbox*)new HitCone(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, angle, slice_prop, type);
    for(unsigned int i = animation_start; i <= animation_end && i < animation_num; i++){
        this->animations[animation_num]->addHitbox(hitbox);
    }
}

/** Sets the X posision
 * @param x The X coordinate
 */
void Object::setX(float x){
    this->x = x;
}

/** Sets the Y posision
 * @param y The Y coordinate
 */
void Object::setY(float y){
    this->y = y;
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

/** Sets the animation number
 * @param animation_num The animation number
 */
void Object::setAnimation(unsigned int animation_num){
    if(this->active_animation != animation_num){
        this->active_animation = animation_num;
        this->animations[active_animation]->start();
    }
}

/** Sets the scale for a single animation
 * @param animation_num The animation number
 * @param x_scale The x scale factor
 * @param y_scale The y scale factor
 */
void Object::setScale(unsigned int animation_num, float x_scale, float y_scale){
    this->animations[animation_num]->setScale(x_scale, y_scale);
}

/** Sets the scale for all animations
 * @param x_scale The x scale factor
 * @param y_scale The y scale factor
 */
void Object::setScale(float x_scale, float y_scale){
    for(unsigned int i = 0; i < this->animation_num; i++){
        this->animations[i]->setScale(x_scale, y_scale);
    }
}

/** Sets the visibility of the current animation state
 * @return If the object is active
 */
void Object::setActive(bool active){
    this->active = active;
}

/** Sets the visibility of the current animation state
 * @return If the object is active
 */
void Object::setVisible(bool visible){
    this->visible = visible;
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

/** Calculates any actions taken; should be overridden by children if used
 * @param event The event being interpreted
 */
void Object::action(SDL_Event* event){
    return;
}

/** Called during the process step; performs object processing calculations
 */
void Object::_process(uint32_t delta){
    this->process();

    //Updating old X & Y values
    this->old_x = this->x;
    this->old_y = this->y;

    //Updating environmental bump
    this->env_bump = false;

    //Updating X values
    this->xV = this->xA + (this->xV * (1 - this->friction) * delta);
    if(this->xV < 0.1 && this->xV > -0.1){
        this->xV = 0;
    }
    this->x += this->xV;
    this->xA = 0;

    //Updating Y values
    this->yV = this->yA + (this->yV * (1 - this->friction) * delta);
    if(this->yV < 0.1 && this->yV > -0.1){
        this->yV = 0;
    }
    this->y += this->yV;
    this->yA = 0;
}

/** Called during the process step by _process; space for users to override with custom processing logics
 */
void Object::process(){
}

/** Called during the draw step
 * @param window The window that content is being drawn to
 */
void Object::draw(SDL_Renderer* renderer){
    this->animations[active_animation]->draw(renderer);
}

/** Called on object collision; should be overridden by children if you want collision logic.
 * @param other The other object
 * @param this_hitbox The hitbox that collided from this object
 * @param other_hitbox The hitbox that collided from the other object
 */
void Object::onCollide(Object* other, Hitbox* this_hitbox, Hitbox* other_hitbox){
	printf("Collided!\n");
}