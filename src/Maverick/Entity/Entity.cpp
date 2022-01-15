#include "./Entity.hpp"
#include "../Zone/Zone.hpp"
#include "../Engine/Engine.hpp"
extern Engine* engine;

/** Object parameterized constructor
 * @param start_x The starting X location of the object
 * @param start_y The starting Y location of the objcet
 * @param friction The object's coefficient of friction
 * @param draw_layer The draw layer of the object
 * @param animation_name The number of animations
 */
Entity::Entity(const char* name, float start_x, float start_y, int draw_layer){
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
    this->active_animation = nullptr;
    this->animations = nullptr;

    //Initializing attributes
    this->attr = new AttributeHash(64);
}

/** Destructor for objects
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

/** Gets the name of the object
 * @return The name of the object
 */
char* Entity::getName(){
    return this->name;
}

/** Gets the X value of the object
 * @return The X value of the object
 */
double Entity::getX(){
    return this->x;
}

/** Gets the Y value of the object
 * @return The Y value of the object
 */
double Entity::getY(){
    return this->y;
}

/** Gets the width of the object
 * @return The width of the object
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

/** Gets the width of the object
 * @return The width of the object
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

/** Gets the draw layer of the object
 * @return The draw layer of the object
 */
int Entity::getDrawLayer(){
    return this->draw_layer;
}

/** Gets the draw axis of the object
 * @return The draw axis of the object
 */
double Entity::getDrawAxis(){
    if(this->active_animation == nullptr){
        return 0;
    }
    return this->active_animation->getDrawAxis();
}

/** Gets the hitboxes of the current animation state
 * @return The HitboxList containing the object's current hitboxes
 */
HitboxList* Entity::getHitboxes(){
    if(this->active_animation == nullptr){
        return nullptr;
    }
    return this->active_animation->getHitboxes();
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
int Entity::addAnimationSequence(const char* animation_name, unsigned int keytime, unsigned int iter){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }
    unsigned int ret = 0;
    for(int i = 0; i < iter; i++){
        ret |= animation->addAnimationSequence(keytime);
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

/** Sets the sprite set for a given animation in the object
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

/** Sets the sprite set for a given animation in the object
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

/** Sets the sprite set for all animations in the object
 * @param sprite_set The sprite set you'd like to swap all object animations to
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

/** Adds a hitbox to a given animation on either the spepcified sprite of an animation or the last-added sprite of the animation (if -1)
 * @param animation_name The animation name
 * @param shape The hitbox shape
 * @param x_offset The X offset of the hitbox
 * @param y_offset The Y offset of the hitbox
 * @param x_element The X width/radius of the hitbox
 * @param y_element The Y width/radius of the hitbox
 * @param type The type flags of the hitbox
 * @param sprite_num The sprite number that is being used
 * @return 0 on success, -1 if the animation doesn't exist
 */
int Entity::addHitbox(const char* animation_name, HITBOX_SHAPE shape, double x_offset, double y_offset, double x_element,
                       double y_element, unsigned int type, int sprite_num){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }

    Hitbox* hitbox;
    //We should never hit the CONE in this case.
    if(shape == RECT){
        hitbox = (Hitbox*)new HitRect(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, type);
    }
    else{
        hitbox = (Hitbox*)new HitEllipse(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, type);
    }

    animation->addHitbox(hitbox, sprite_num);
    return 0;
}

/** Adds a hitbox to a given animation on all sprites
 * @param animation_name The animation name
 * @param shape The hitbox shape
 * @param x_offset The X offset of the hitbox
 * @param y_offset The Y offset of the hitbox
 * @param x_element The X width/radius of the hitbox
 * @param y_element The Y width/radius of the hitbox
 * @param type The type flags of the hitbox
 * @return 0 on success, -1 if the animation doesn't exist
 */
int Entity::addHitbox(const char* animation_name, HITBOX_SHAPE shape, double x_offset, double y_offset, double x_element, double y_element, unsigned int type){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }

    Hitbox* hitbox;
    //We should never hit the CONE in this case.
    if(shape == RECT){
        hitbox = (Hitbox*)new HitRect(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, type);
    }
    else{
        hitbox = (Hitbox*)new HitEllipse(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, type);
    }

    animation->addHitbox(hitbox);
    return 0;
}

/** Adds a hitbox to all loaded animations
 * @param shape The hitbox shape
 * @param x_offset The X offset of the hitbox
 * @param y_offset The Y offset of the hitbox
 * @param x_element The X width/radius of the hitbox
 * @param y_element The Y width/radius of the hitbox
 * @param type The type flags of the hitbox
 * @return 0 on success, -1 if no animations exist
 */
int Entity::addHitbox(HITBOX_SHAPE shape, double x_offset, double y_offset, double x_element, double y_element, unsigned int type){
    if(animations == nullptr){
        return -1;
    }

    Hitbox* hitbox;
    //We should never hit the CONE in this case.
    if(shape == RECT){
        hitbox = (Hitbox*)new HitRect(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, type);
    }
    else{
        hitbox = (Hitbox*)new HitEllipse(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, type);
    }

    AnimationList* animation_cursor = animations;
    while(animation_cursor != nullptr){
        animation_cursor->animation->addHitbox(hitbox);
        animation_cursor = animation_cursor->next;
    }

    return 0;
}

/** Adds a hitbox to a given animation (including angles and ratios) on either the specified sprite of an animation or the last-added sprite of the animation (if -1). Due to the fact cones are the only ones using angles and ratios, this is currently just cones.
 * @param animation_name The animation name
 * @param type The hitbox type
 * @param x_offset The X offset of the hitbox
 * @param y_offset The Y offset of the hitbox
 * @param x_element The X width/radius of the hitbox
 * @param y_element The Y width/radius of the hitbox
 * @param type The type flags of the hitbox
 * @param angle The slice angle
 * @param slice_prop The slice proportion for the hitbox
 * @param sprite_num The sprite number that is being used
 * @return 0 on success, -1 if the animation doesn't exist
 */
int Entity::addHitbox(const char* animation_name, HITBOX_SHAPE shape, double x_offset, double y_offset,
                       double x_element, double y_element, unsigned int type, float angle, float slice_prop, int sprite_num){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }

    Hitbox* hitbox = (Hitbox*)new HitCone(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, angle, slice_prop, type);
    animation->addHitbox(hitbox, sprite_num);
    return 0;
}

/** Adds a hitbox to a given animation (including angles and ratios) on all sprites. Due to the fact cones are the only ones using angles and ratios, this is currently just cones.
 * @param animation_name The animation name
 * @param type The hitbox type
 * @param x_offset The X offset of the hitbox
 * @param y_offset The Y offset of the hitbox
 * @param x_element The X width/radius of the hitbox
 * @param y_element The Y width/radius of the hitbox
 * @param type The type flags of the hitbox
 * @param angle The slice angle
 * @param slice_prop The slice proportion for the hitbox
 * @return 0 on success, -1 if the animation doesn't exist
 */
int Entity::addHitbox(const char* animation_name, HITBOX_SHAPE shape, double x_offset, double y_offset,
                       double x_element, double y_element, unsigned int type, float angle, float slice_prop){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }

    Hitbox* hitbox = (Hitbox*)new HitCone(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, angle, slice_prop, type);
    animation->addHitbox(hitbox);
    return 0;
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
 * @return 0 on success, -1 if the animation doesn't exist
 */
int Entity::addHitbox(HITBOX_SHAPE shape, double x_offset, double y_offset,
                       double x_element, double y_element, unsigned int type, float angle, float slice_prop){
    if(animations == nullptr){
        return -1;
    }
    
    Hitbox* hitbox = (Hitbox*)new HitCone(&(this->x), &(this->y), x_offset, y_offset, x_element, y_element, angle, slice_prop, type);
    AnimationList* animation_cursor = animations;
    while(animation_cursor != nullptr){
        animation_cursor->animation->addHitbox(hitbox);
        animation_cursor = animation_cursor->next;
    }
    return 0;
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
int Entity::setScale(const char* animation_name, double x_scale, double y_scale){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }
    animation->setScale(x_scale, y_scale);
    return 0;
}

/** Sets the scale for all animations
 * @param x_scale The x scale factor
 * @param y_scale The y scale factor
 */
void Entity::setScale(double x_scale, double y_scale){
    AnimationList* animation_cursor = animations;
    while(animation_cursor != nullptr){
        animation_cursor->animation->setScale(x_scale, y_scale);
        animation_cursor = animation_cursor->next;
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

/** Gets the object type
 * @return A uint32_t representation of the object type set in the constructor
 */
uint16_t Entity::getType(){
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
    //Write the object type
    uint16_t object_type = EndianSwap(&this->type);
    fwrite(&object_type, 2, 1, file);

    //Identifier
    uint16_t identifier_len = strlen(this->name);
    uint16_t identifier_len_swapped = EndianSwap(&identifier_len);
    fwrite(&identifier_len_swapped, sizeof(identifier_len_swapped), 1, file);
    fwrite(this->name, 1, identifier_len, file);

    double write_x = this->x - base_zone->getGlobalX();
    double write_y = this->y - base_zone->getGlobalY();

    fwrite(&write_x, sizeof(write_x), 1, file);
    fwrite(&write_y, sizeof(write_y), 1, file);

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
            case ATTR_DATA_TYPE::BOOL:
            case ATTR_DATA_TYPE::CHAR:{
                bool attr_val = attr_list->val == 0;
                fwrite(&attr_val, sizeof(attr_val), 1, file);
                break;
            }

            //Writing ints/uints (8 bytes)
            case ATTR_DATA_TYPE::INT:
            case ATTR_DATA_TYPE::UINT:{
                uint64_t attr_val = (uint64_t)attr_list->val;
                fwrite(&attr_val, sizeof(attr_val), 1, file);
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

    return serializeExtendedData(file, base_zone);
}

/** This function should be overridden by children that wish to write additional
 *  data beyond the default saved info; everything needed by the base class is
 *  already saved by the point this is called, so it will just return 0 (success).
 */
int Entity::serializeExtendedData(FILE* file, Zone* base_zone){
    return 0;
}

/** Saves the resources of the entity to a char*'s (which should be freed upon return)
 * @param file The pointer to the open file to write to
 * @param written_sprites The set of sprites that have already been written to file
 * @param written_audio The set of audio assets that have already been written to file
 * @param written_music The set of music assets that have already been written to file (used just by objects that handle music)
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

    return serializeExtendedAssets(file, written_sprites, written_audio, written_music);
}

/** This function should be overridden by children that wish to write additional
 *  assets beyond the default saved info; everything needed by the base class is
 *  already saved by the point this is called, so it will just return 0 (success).
 */
int Entity::serializeExtendedAssets(FILE* file, std::unordered_set<std::string>& sprite_set, std::unordered_set<std::string>& audio_set, std::unordered_set<std::string>& music_set){
    return 0;
}