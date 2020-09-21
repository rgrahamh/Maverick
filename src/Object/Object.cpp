#include "./Object.hpp"

/** Object parameterized constructor
 * @param start_x The starting X location of the object
 * @param start_y The starting Y location of the objcet
 * @param friction The object's coefficient of friction
 * @param draw_layer The draw layer of the object
 * @param animation_num The number of animations
 */
Object::Object(float start_x, float start_y, float friction, unsigned int draw_layer, unsigned int animation_num){
    this->x = start_x;
    this->y = start_y;
    this->old_x = start_x;
    this->old_y = start_y;
    this->friction = friction;
    this->draw_layer = draw_layer;
    this->active_animation = 0;
    this->animations = (Animation**)calloc(sizeof(Animation*), animation_num);
    for(int i = 0; i < animation_num; i++){
        animations[i] = new Animation(&x, &y, true);
    }
}

/** Destructor for objects
 */
Object::~Object(){
    for(int i = 0; i < animation_num; i++){
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

/** Gets the width of the object
 * @return The width of the object
 */
float Object::getWidth(){
    return this->animations[active_animation]->getSprite()->getGlobalBounds().width;
}

/** Gets the width of the object
 * @return The width of the object
 */
float Object::getHeight(){
    return this->animations[active_animation]->getSprite()->getGlobalBounds().width;
}

/** Gets the draw layer of the object
 * @return The draw layer of the object
 */
unsigned int Object::getDrawLayer(){
    return this->draw_layer;
}

/** Gets the hitboxes of the current animation state
 * @return The HitboxLst containing the object's current hitboxes
 */
HitboxLst* Object::getHitboxes(){
    return this->animations[this->animation_num]->getHitboxes();
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

/** Adds a hitbox to a given animation on either the spepcified sprite of an animation or the last-added sprite of the animation (by default)
 * @param animation_num The animation number
 * @param type The hitbox type
 * @param x_offset The X offset of the hitbox
 * @param y_offset The Y offset of the hitbox
 * @param x_element The X width/radius of the hitbox
 * @param y_element The Y width/radius of the hitbox
 * @param sprite_num The sprite number that is being used
 */
void Object::addHitbox(unsigned int animation_num, HITBOX_TYPE type, float x_offset, float y_offset, float x_element, float y_element, int sprite_num){
    Hitbox* hitbox;
    switch(type){
        case RECT:
            hitbox = (Hitbox*)new HitRect(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element);
            break;
        case ELLIPSE:
            hitbox = (Hitbox*)new HitEllipse(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element);
            break;
    }

    this->animations[this->animation_num]->addHitbox(hitbox, sprite_num);
}

/** Adds a hitbox to a given animation (including angles and ratios) on either the spepcified sprite of an animation or the last-added sprite of the animation (by default)
 * @param animation_num The animation number
 * @param type The hitbox type
 * @param x_offset The X offset of the hitbox
 * @param y_offset The Y offset of the hitbox
 * @param x_element The X width/radius of the hitbox
 * @param y_element The Y width/radius of the hitbox
 * @param slice_prop The slice angle
 * @param slice_prop The slice proportion for the hitbox
 * @param sprite_num The sprite number that is being used
 * @param 
 */
void Object::addHitbox(unsigned int animation_num, HITBOX_TYPE type, float x_offset, float y_offset, float x_element, float y_element, float angle, float slice_prop, int sprite_num){
    Hitbox* hitbox = (Hitbox*)new HitCone(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, angle, slice_prop);
    this->animations[this->animation_num]->addHitbox(hitbox, sprite_num);
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
    for(unsigned int i = 0; i < animation_num; i++){
        this->animations[i]->setScale(x_scale, y_scale);
    }
}

/** Calculates any actions taken; should be overridden by children if used
 * @param event The event being interpreted
 */
void Object::action(sf::Event event){
    return;
}

/** Called during the process step; performs object processing calculations
 */
void Object::_process(){
    this->process();

    //Updating old X & Y values
    this->old_x = this->x;
    this->old_y = this->y;

    //Updating X values
    this->xV = this->xA + (this->xV * (1 - this->friction));
    this->x += this->xV;
    this->xA = 0;

    //Updating Y values
    this->yV = this->yA + (this->yV * (1 - this->friction));
    this->y += this->yV;
    this->yV = 0;
}

/** Called during the process step by _process; space for users to override with custom processing logics
 */
void Object::process(){
}

/** Called during the draw step
 * @param window The window that content is being drawn to
 */
void Object::draw(sf::RenderWindow* window){
    this->animations[active_animation]->draw(window);
}

/** Called on object collision; should be overridden by children if you want collision logic.
 * @param other The other object
 * @param this_hitbox The hitbox that collided from this object
 * @param other_hitbox The hitbox that collided from the other object
 */
void Object::onCollide(Object* other, Hitbox* this_hitbox, Hitbox* other_hitbox){
    return;
}