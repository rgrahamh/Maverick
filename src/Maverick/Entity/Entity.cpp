#include "./Entity.hpp"
#include "../Zone/Zone.hpp"
#include "../Engine/Engine.hpp"
extern Engine* engine;

/** Entity parameterized constructor
 * @param name The name of the entity
 * @param start_x The starting X location of the entity
 * @param start_y The starting Y location of the entity
 * @param draw_layer The draw layer of the entity
 */
Entity::Entity(const char* name, float start_x, float start_y, int draw_layer){
    this->name = StrDeepCopy(name);
    
    //Initializing position, velocity, and acceleration
    this->x = start_x;
    this->y = start_y;

    this->draw_layer = draw_layer;

    this->num_animations = 0;

    //Initializing animation/visibility attributes
    this->active = true;
    this->visible = true;
    this->active_animation = nullptr;
    this->animations = nullptr;
    this->hitbox_immunity = nullptr;

    //Initializing attributes
    this->attr = new AttributeHash(64);
}

/** Destructor for entities
 */
Entity::~Entity(){
    while(animations != nullptr){
        delete animations->animation;
        AnimationList* tmp = animations;
        animations = animations->next;
        delete tmp;
    }
    free(name);
}

/** Gets the name of the entity
 * @return The name of the entity
 */
char* Entity::getName(){
    return this->name;
}

/** Gets the X value of the entity
 * @return The X value of the entity
 */
double Entity::getX(){
    return this->x;
}

/** Gets the Y value of the entity
 * @return The Y value of the entity
 */
double Entity::getY(){
    return this->y;
}

/** Gets the width of the entity
 * @return The width of the entity
 */
float Entity::getWidth(){
    if(this->active_animation == nullptr){
        return 0;
    }
    Sprite* sprite = this->active_animation->getSprite();
    if(sprite == nullptr){
        return 0;
    }
    return sprite->rect->w;
}

/** Gets the width of the entity
 * @return The width of the entity
 */
float Entity::getHeight(){
    if(this->active_animation == nullptr){
        return 0;
    }
    Sprite* sprite = this->active_animation->getSprite();
    if(sprite == nullptr){
        return 0;
    }
    return sprite->rect->h;
}

/** Gets the draw layer of the entity
 * @return The draw layer of the entity
 */
int16_t Entity::getDrawLayer(){
    return this->draw_layer;
}

/** Gets the draw axis of the entity
 * @return The draw axis of the entity
 */
double Entity::getDrawAxis(){
    if(this->active_animation == nullptr){
        return 0;
    }
    return this->active_animation->getDrawAxis();
}

/** Gets the hitboxes of the current animation state
 * @return The HitboxList containing the entity's current hitboxes
 */
HitboxList* Entity::getHitboxes(){
    if(this->active_animation == nullptr){
        return nullptr;
    }
    return this->active_animation->getHitboxes();
}

/** Sets if the current entity is active (can be interacted with)
 * @return If the entity is active
 */
bool Entity::isActive(){
    return this->active;
}

/** Sets if the current entity is visible (will/won't be drawn)
 * @return If the entity is active
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

/** Creates a new animation (you MUST do this before adding hitboxes/sprites)
 * @param animation_name The name of the new animation
 */
int Entity::addAnimation(const char* animation_name, uint32_t num_sprite_sets){
    //If the animation already exists, just return
    if(findAnimation(animation_name) != nullptr){
        return -1;
    }

    Animation* new_animation = new Animation(animation_name, &x, &y, num_sprite_sets);
    AnimationList* new_animation_list = new AnimationList;
    new_animation_list->animation = new_animation;
    new_animation_list->next = nullptr;

    if(animations == nullptr){
        animations = new_animation_list;
    }
    else{
        new_animation_list->next = animations;
        animations = new_animation_list;
    }
    this->num_animations++;
    return 0;
}

/** Adds a sprite to a given animation
 * @param animation_name The animation name
 * @param sprite_path The filepath to the sprite you're adding
 * @param keytime The number of frames until the animation progresses
 * @param x_offset The X offset of the sprite
 * @param y_offset The Y offset of the sprite
 * @param width The width of the sprite (default if -1)
 * @param height The height of the sprite (default if -1)
 * @return 0 on success, -1 if the animation doesn't exist
 */ 
int Entity::addFrame(const char* animation_name, unsigned int keytime, unsigned int iter){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }
    unsigned int ret = 0;
    for(unsigned int i = 0; i < iter; i++){
        ret |= animation->addFrame(keytime);
    }
    return ret; 
}

int Entity::addSprite(const char* animation_name, const char* sprite_set, const char* sprite_path, int x_offset, int y_offset, int width, int height){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }

    //If the animation doesn't have the sprite set, add it
    if(!animation->hasSpriteSet(sprite_set)){
        animation->addSpriteSet(sprite_set);
    }

    return animation->addSprite(sprite_set, sprite_path, x_offset, y_offset, width, height);
}

/** Sets the sprite set for a given animation in the entity
 * @param animation_name The animation set you'd like to add the sprite set to
 * @param sprite_set The sprite set you'd like to add
 * @return -1 if the sprite set add failed, 0 otherwise
 */
int Entity::addSpriteSet(const char* animation_name, const char* sprite_set){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }

    return animation->addSpriteSet(sprite_set);
}

/** Sets the sprite set for a given animation in the entity
 * @param animation_name The animation set you'd like to swap the sprite set for
 * @param sprite_set The sprite set you'd like to swap to
 * @return -1 if the swap failed, 0 otherwise
 */
int Entity::setSpriteSet(const char* animation_name, const char* sprite_set){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }

    return animation->setSpriteSet(sprite_set);
}

/** Sets the sprite set for all animations in the entity
 * @param sprite_set The sprite set you'd like to swap all entity animations to
 * @return -1 if any setSpriteSet() calls failed, 0 otherwise
 */
int Entity::setSpriteSet(const char* sprite_set){
    int ret = 0;
    AnimationList* cursor = animations;
    while(cursor != nullptr){
        ret |= cursor->animation->setSpriteSet(sprite_set);
        cursor = cursor->next;
    }

    return ret;
}

/** Sets the draw axis for the specified animation in the entity
 * @param animation_name The name of the animation you'd like to set the draw axis of
 * @param draw_axis The draw axis (offset from the the top of the sprite)
 */
int Entity::setDrawAxis(const char* animation_name, double draw_axis){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }

    animation->setDrawAxis(draw_axis);
    return 0;
}

/** Sets the draw axis for all animations in the entity
 * @param draw_axis The draw axis (offset from the the top of the sprite)
 */
int Entity::setDrawAxis(double draw_axis){
    if(animations == nullptr){
        return -1;
    }

    AnimationList* cursor = animations;
    while(cursor != nullptr){
        cursor->animation->setDrawAxis(draw_axis);
        cursor = cursor->next;
    }

    return 0;
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
int Entity::addHitbox(const char* animation_name, HITBOX_SHAPE shape, double x_offset, double y_offset, double z_offset, double x_element,
                       double y_element, double depth, unsigned int type, int sprite_num, int hitbox_group, uint32_t immunity_timer){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }

    Hitbox* hitbox;
    //We should never hit the CONE in this case.
    if(shape == RECT){
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
void Entity::addHitboxImmunity(Entity* other, Hitbox* hitbox){
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
bool Entity::checkHitboxImmunity(Entity* other, Hitbox* hitbox){
    if(this->hitbox_immunity == nullptr){
        return false;
    }

    HitboxImmunityList* cursor = this->hitbox_immunity;
    int32_t hitbox_group = cursor->hitbox_group;
    while(cursor != nullptr){
        if(cursor->hitbox == hitbox || (other == cursor->entity && cursor->hitbox_group == hitbox_group)){
            return true;
        }

        cursor = cursor->next;
    }

    return false;
}

/** Cleans up the hitbox immunity list; should be run in the _process step
 */
void Entity::cleanupHitboxImmunity(uint64_t delta){
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

/** Adds a sound to a given animation
 * @param animation_name The animation name
 * @param sprite_path The filepath to the sprite you're adding
 * @param keytime The number of frames until the animation progresses
 * @param x_offset The X offset of the sprite
 * @param y_offset The Y offset of the sprite
 * @param width The width of the sprite (default if -1)
 * @param height The height of the sprite (default if -1)
 * @return 0 on success, -1 if the animation doesn't exist
 */
int Entity::addSound(const char* animation_name, const char* sound_path, int sequence_num){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }
    return animation->addSound(engine->getSound(sound_path), sequence_num);
}

/** Sets the X posision
 * @param x The X coordinate
 */
void Entity::setX(double x){
    this->x = x;
}

/** Sets the Y posision
 * @param y The Y coordinate
 */
void Entity::setY(double y){
    this->y = y;
}

/** Sets the animation number
 * @param animation_name The animation name
 * @return 0 on success, -1 if the animation doesn't exist
 */
int Entity::setAnimation(const char* animation_name){
    Animation* animation = findAnimation(animation_name);
    if(animation != nullptr && animation != this->active_animation){
        this->active_animation = animation;
        this->active_animation->start();
        return 0;
    }
    else{
        return -1;
    }
}

/** Sets the scale for a single animation
 * @param animation_name The animation name
 * @param x_scale The x scale factor
 * @param y_scale The y scale factor
 * @return 0 on success, -1 if the animation doesn't exist
 */
int Entity::setSize(const char* animation_name, float width, float height){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }
    animation->setSize(width, height);
    return 0;
}

/** Sets the scale for all animations
 * @param width The x scale factor
 * @param height The y scale factor
 */
void Entity::setSize(float width, float height){
    AnimationList* animation_cursor = animations;
    while(animation_cursor != nullptr){
        animation_cursor->animation->setSize(width, height);
        animation_cursor = animation_cursor->next;
    }
}

/** Sets the activation of the current animation state
 * @return If the entity is active
 */
void Entity::setActive(bool active){
    this->active = active;
}

/** Sets the visibility of the current animation state
 * @return If the entity is active
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

/** Gets the entity type
 * @return A uint32_t representation of the entity type set in the constructor
 */
uint32_t Entity::getType(){
    return this->type;
}

Animation* Entity::findAnimation(const char* animation_name){
    AnimationList* animation_cursor = animations;
    while(animation_cursor != nullptr){
        if(strcmp(animation_cursor->animation->getName(), animation_name) == 0){
            break;
        }
        animation_cursor = animation_cursor->next;
    }

    if(animation_cursor == nullptr){
        return nullptr;
    }
    return animation_cursor->animation;
}

int Entity::serializeData(FILE* file, Zone* base_zone){
    if(file == nullptr){
        return -1;
    }

    //GENERAL SECTION
    //Write the entity type
    uint32_t entity_type = EndianSwap(&this->type);
    fwrite(&entity_type, sizeof(entity_type), 1, file);

    //Identifier
    uint16_t identifier_len = strlen(this->name);
    uint16_t identifier_len_swapped = EndianSwap(&identifier_len);
    fwrite(&identifier_len_swapped, sizeof(identifier_len_swapped), 1, file);
    fwrite(this->name, 1, identifier_len, file);

    //Draw layer
    int16_t draw_layer_swap = EndianSwap(&this->draw_layer);
    fwrite(&draw_layer_swap, sizeof(draw_layer_swap), 1, file);

    //ATTRIBUTE SECTION
    AttributeHashEntry* attr_list;
    uint32_t num_entries = attr->dump(&attr_list);
    num_entries = EndianSwap(&num_entries);
    fwrite(&num_entries, sizeof(num_entries), 1, file);

    while(attr_list != nullptr){
        //Attribute type
        uint16_t attr_type = attr_list->type;
        attr_type = EndianSwap(&attr_type);
        fwrite(&attr_type, sizeof(attr_type), 1, file);

        //The attribute
        switch(attr_list->type){
            //Writing chars & bools (1 byte)
            case ATTR_DATA_TYPE::BOOL_PTR:
            case ATTR_DATA_TYPE::CHAR_PTR:{
                uint8_t* attr_val = (uint8_t*)attr_list->val;
                fwrite(attr_val, sizeof(attr_val), 1, file);
                break;
            }

            //Writing ints/uints (8 bytes)
            case ATTR_DATA_TYPE::INT_PTR:
            case ATTR_DATA_TYPE::UINT_PTR:{
                uint64_t* attr_val = (uint64_t*)attr_list->val;
                fwrite(attr_val, sizeof(attr_val), 1, file);
                break;
            }

            //Writing stings (? bytes)
            case ATTR_DATA_TYPE::STRING:{
                char* attr_str = (char*)attr_list->val;
                uint16_t attr_val_len = strlen(attr_str);
                uint16_t attr_val_len_swapped = EndianSwap(&attr_val_len);
                fwrite(&attr_val_len_swapped, sizeof(attr_val_len_swapped), 1, file);
                fwrite(attr_str, 1, attr_val_len, file);
                break;
            }
        }
    }

    //ANIMATION SECTION
    uint16_t num_animations = EndianSwap(&this->num_animations);
    fwrite(&num_animations, sizeof(num_animations), 1, file);

    AnimationList* animation_cursor = this->animations;
    while(animation_cursor != nullptr){
        animation_cursor->animation->serializeData(file);
        animation_cursor = animation_cursor->next;
    }

    if(this->active_animation != nullptr){
        const char* starting_animation = this->active_animation->getName();
        uint16_t starting_animation_len = strlen(starting_animation);
        uint16_t starting_animation_len_swapped = EndianSwap(&starting_animation_len);
        fwrite(&starting_animation_len_swapped, sizeof(starting_animation_len_swapped), 1, file);
        fwrite(starting_animation, 1, starting_animation_len, file);
    }

    return this->serializeExtendedData(file, base_zone);
}

/** Saves the resources of the entity to a char*'s (which should be freed upon return)
 * @param file The pointer to the open file to write to
 * @param written_sprites The set of sprites that have already been written to file
 * @param written_audio The set of audio assets that have already been written to file
 * @param written_music The set of music assets that have already been written to file (used just by entities that handle music)
 * @return The number of bytes that *buff_ptr is
 */
int Entity::serializeAssets(FILE* file, std::unordered_set<std::string>& written_sprites, std::unordered_set<std::string>& written_audio, std::unordered_set<std::string>& written_music){
    if(file == nullptr){
        return -1;
    }

    AnimationList* animation_cursor = animations;
    while(animation_cursor != nullptr){
        animation_cursor->animation->serializeAssets(file, written_sprites, written_audio);
        animation_cursor = animation_cursor->next;
    }

    return this->serializeExtendedAssets(file, written_sprites, written_audio, written_music);
}