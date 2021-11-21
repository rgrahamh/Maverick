#include "./Entity.hpp"

/** Object parameterized constructor
 * @param start_x The starting X location of the object
 * @param start_y The starting Y location of the objcet
 * @param friction The object's coefficient of friction
 * @param draw_layer The draw layer of the object
 * @param animation_num The number of animations
 */
Entity::Entity(const char* name, float start_x, float start_y, unsigned int animation_num, int draw_layer){
    int name_len = strlen(name);
    this->name = (char*)malloc(name_len + 1);
    memcpy(this->name, name, name_len);
    this->name[name_len] = '\0';
    
    //Initializing position, velocity, and acceleration
    this->x = start_x;
    this->y = start_y;

    this->draw_layer = draw_layer;

    //Initializing animation/visibility attributes
    this->active = true;
    this->visible = true;
    this->active_animation = 0;
    this->animation_num = animation_num;
    if(animation_num > 0){
        this->animations = (Animation**)malloc(sizeof(Animation*) * animation_num);
        for(unsigned int i = 0; i < animation_num; i++){
            animations[i] = new Animation(&(this->x), &(this->y));
        }
    }

    //Initializing attributes
    this->attr = new HashTable(64);
}

/** Destructor for objects
 */
Entity::~Entity(){
    for(unsigned int i = 0; i < animation_num; i++){
        delete animations[i];
    }
    free(animations);
    free(name);
}

/** Gets the name of the object
 * @return The name of the object
 */
char* Entity::getName(){
    return this->name;
}

/** Gets the X value of the object
 * @return The X value of the object
 */
float Entity::getX(){
    return this->x;
}

/** Gets the Y value of the object
 * @return The Y value of the object
 */
float Entity::getY(){
    return this->y;
}

/** Gets the width of the object
 * @return The width of the object
 */
float Entity::getWidth(){
    return this->animations[active_animation]->getSprite()->rect->w;
}

/** Gets the width of the object
 * @return The width of the object
 */
float Entity::getHeight(){
    return this->animations[active_animation]->getSprite()->rect->h;
}

/** Gets the draw layer of the object
 * @return The draw layer of the object
 */
int Entity::getDrawLayer(){
    return this->draw_layer;
}

/** Gets the draw axis of the object
 * @return The draw axis of the object
 */
float Entity::getDrawAxis(){
    return (this->animation_num == 0)? 0 : this->animations[active_animation]->getDrawAxis();
}

/** Gets the hitboxes of the current animation state
 * @return The HitboxList containing the object's current hitboxes
 */
HitboxList* Entity::getHitboxes(){
    return this->animations[active_animation]->getHitboxes();
}

/** Sets if the current object is active (can be interacted with)
 * @return If the object is active
 */
bool Entity::isActive(){
    return this->active;
}

/** Sets if the current object is visible (will/won't be drawn)
 * @return If the object is active
 */
bool Entity::isVisible(){
    return this->visible;
}

/** Gets a pointer to an attribute
 * @param key The attribute you're looking for
 * @return A pointer to the var if found, NULL otherwise
 */
void* Entity::getAttr(const char* key){
    return this->attr->get(key);
}

/** Adds a sprite to a given animation
 * @param animation_num The animation number
 * @param sprite_path The filepath to the sprite you're adding
 * @param keytime The number of frames until the animation progresses
 * @param x_offset The X offset of the sprite
 * @param y_offset The Y offset of the sprite
 * @param width The width of the sprite (default if -1)
 * @param height The height of the sprite (default if -1)
 */ 
void Entity::addSprite(unsigned int animation_num, const char* sprite_path, unsigned int keytime, int x_offset, int y_offset, int width, int height){
    this->animations[animation_num]->addFrame(sprite_path, keytime, x_offset, y_offset, width, height);
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
void Entity::addHitbox(unsigned int animation_num, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element,
                       float y_element, unsigned int type, int sprite_num){
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
void Entity::addHitbox(unsigned int animation_num, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type){
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
void Entity::addHitbox(unsigned int animation_start, unsigned int animation_end, HITBOX_SHAPE shape,
                       float x_offset, float y_offset, float x_element, float y_element, unsigned int type){
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
void Entity::addHitbox(unsigned int animation_num, HITBOX_SHAPE shape, float x_offset, float y_offset,
                       float x_element, float y_element, unsigned int type, float angle, float slice_prop, int sprite_num){
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
void Entity::addHitbox(unsigned int animation_num, HITBOX_SHAPE shape, float x_offset, float y_offset,
                       float x_element, float y_element, unsigned int type, float angle, float slice_prop){
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
void Entity::addHitbox(unsigned int animation_start, unsigned int animation_end, HITBOX_SHAPE shape, float x_offset, float y_offset,
                       float x_element, float y_element, unsigned int type, float angle, float slice_prop){
    Hitbox* hitbox = (Hitbox*)new HitCone(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, angle, slice_prop, type);
    for(unsigned int i = animation_start; i <= animation_end && i < animation_num; i++){
        this->animations[animation_num]->addHitbox(hitbox);
    }
}

/** Sets the X posision
 * @param x The X coordinate
 */
void Entity::setX(float x){
    this->x = x;
}

/** Sets the Y posision
 * @param y The Y coordinate
 */
void Entity::setY(float y){
    this->y = y;
}

/** Sets the animation number
 * @param animation_num The animation number
 */
void Entity::setAnimation(unsigned int animation_num){
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
void Entity::setScale(unsigned int animation_num, float x_scale, float y_scale){
    this->animations[animation_num]->setScale(x_scale, y_scale);
}

/** Sets the scale for all animations
 * @param x_scale The x scale factor
 * @param y_scale The y scale factor
 */
void Entity::setScale(float x_scale, float y_scale){
    for(unsigned int i = 0; i < this->animation_num; i++){
        this->animations[i]->setScale(x_scale, y_scale);
    }
}

/** Sets the scale for a single animation
 * @param animation_num The animation number
 * @param x_scale The x scale factor
 * @param y_scale The y scale factor
 */
void Entity::setSize(unsigned int animation_num, float width, float height){
    this->animations[animation_num]->setSize(width, height);
}

/** Sets the scale for all animations
 * @param width The x scale factor
 * @param height The y scale factor
 */
void Entity::setSize(float width, float height){
    for(unsigned int i = 0; i < this->animation_num; i++){
        this->animations[i]->setSize(width, height);
    }
}

/** Sets the visibility of the current animation state
 * @return If the object is active
 */
void Entity::setActive(bool active){
    this->active = active;
}

/** Sets the visibility of the current animation state
 * @return If the object is active
 */
void Entity::setVisible(bool visible){
    this->visible = visible;
}

/** Sets the attribute key to the specified val
 * @param key The key you wish to set
 * @param val The val you wish to set
 */
void Entity::setAttr(const char* key, void* val){
    this->attr->add(key, val);
}