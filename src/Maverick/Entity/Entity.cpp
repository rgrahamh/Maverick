#include "./Entity.hpp"
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
    this->attr = new HashTable(64);
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
    if(this->active_animation != nullptr){
        return this->active_animation->getSprite()->rect->w;
    }
    else{
        return 0;
    }
}

/** Gets the width of the object
 * @return The width of the object
 */
float Entity::getHeight(){
    if(this->active_animation != nullptr){
        return this->active_animation->getSprite()->rect->h;
    }
    else{
        return 0;
    }
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
    if(this->active_animation != nullptr){
        return this->active_animation->getDrawAxis();
    }
    else{
        return 0;
    }
}

/** Gets the hitboxes of the current animation state
 * @return The HitboxList containing the object's current hitboxes
 */
HitboxList* Entity::getHitboxes(){
    if(this->active_animation != nullptr){
        return this->active_animation->getHitboxes();
    }
    else{
        return nullptr;
    }
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
int Entity::addAnimation(const char* animation_name){
    //If the animation already exists, just return
    if(findAnimation(animation_name) != nullptr){
        return -1;
    }

    Animation* new_animation = new Animation(animation_name, &x, &y);
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
int Entity::addSprite(const char* animation_name, const char* sprite_path, unsigned int keytime, int x_offset, int y_offset, int width, int height){
    Animation* animation = findAnimation(animation_name);
    if(animation != nullptr){
        return animation->addFrame(sprite_path, keytime, x_offset, y_offset, width, height);
    }
    else{
        return -1;
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
 * @return 0 on success, -1 if the animation doesn't exist
 */
int Entity::addHitbox(const char* animation_name, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element,
                       float y_element, unsigned int type, int sprite_num){
    Animation* animation = findAnimation(animation_name);
    if(animation != nullptr){
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
int Entity::addHitbox(const char* animation_name, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type){
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
int Entity::addHitbox(HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type){
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
int Entity::addHitbox(const char* animation_name, HITBOX_SHAPE shape, float x_offset, float y_offset,
                       float x_element, float y_element, unsigned int type, float angle, float slice_prop, int sprite_num){
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
int Entity::addHitbox(const char* animation_name, HITBOX_SHAPE shape, float x_offset, float y_offset,
                       float x_element, float y_element, unsigned int type, float angle, float slice_prop){
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
int Entity::addHitbox(HITBOX_SHAPE shape, float x_offset, float y_offset,
                       float x_element, float y_element, unsigned int type, float angle, float slice_prop){
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
    if(animation != nullptr){
        return animation->addSound(engine->getSound(sound_path), sequence_num);
    }
    else{
        return -1;
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
int Entity::setScale(const char* animation_name, float x_scale, float y_scale){
    Animation* animation = findAnimation(animation_name);
    if(animation != nullptr){
        animation->setScale(x_scale, y_scale);
        return 0;
    }
    else{
        return -1;
    }
}

/** Sets the scale for all animations
 * @param x_scale The x scale factor
 * @param y_scale The y scale factor
 */
void Entity::setScale(float x_scale, float y_scale){
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
    if(animation != nullptr){
        animation->setSize(width, height);
        return 0;
    }
    else{
        return -1;
    }
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

int Entity::serializeData(FILE* file){
    return 0;
}

/** Saves the resources of the entity to a char*'s (which should be freed upon return)
 * @param file The pointer to the open file to write to
 * @param sprite_set The set of sprites that have already been written to file
 * @param audio_set The set of audio assets that have already been written to file
 * @param music_set The set of music assets that have already been written to file (used just by objects that handle music)
 * @return The number of bytes that *buff_ptr is
 */
int Entity::serializeAssets(FILE* file, std::unordered_set<std::string>& sprite_set, std::unordered_set<std::string>& audio_set, std::unordered_set<std::string>& music_set){
    if(active_animation != nullptr){
        return -1;
    }

    AnimationSeq* sequence_start = active_animation->getSequenceStart();
    AnimationSeq* cursor = sequence_start;
    if(cursor != NULL){
        do{
            //If this asset's not been saved in this file yet
            if(sprite_set.find(std::string(cursor->sprite->name)) == sprite_set.end()){
                //Use the surface from the engine; the animation's actual surface may have transparency set, masking, etc.
                SDL_Surface* surface = engine->getSurface(cursor->sprite->name);

                //Gather necessary info
                //Width/Height are naturally ints (so size varies), meaning we need to truncate first
                uint32_t width = surface->w;
                width = EndianSwap(&width);
                uint32_t height = surface->h;
                height = EndianSwap(&height);

                //Bit depth of the image
                uint8_t depth = surface->format->BitsPerPixel;

                //The RGBA masks
                uint32_t rmask = EndianSwap(&surface->format->Rmask);
                uint32_t gmask = EndianSwap(&surface->format->Gmask);
                uint32_t bmask = EndianSwap(&surface->format->Bmask);
                uint32_t amask = EndianSwap(&surface->format->Amask);

                //Identifier len
                uint16_t identifier_len = strlen(cursor->sprite->name);
                uint16_t identifier_len_swapped = EndianSwap(&identifier_len);
                fwrite(&identifier_len_swapped, 2, 1, file);

                //Identifier
                fwrite(cursor->sprite->name, 1, identifier_len, file);

                //Write the image header info
                fwrite(&width, 4, 1, file);
                fwrite(&height, 4, 1, file);
                fwrite(&depth, 1, 1, file);
                fwrite(&rmask, 4, 1, file);
                fwrite(&gmask, 4, 1, file);
                fwrite(&bmask, 4, 1, file);
                fwrite(&amask, 4, 1, file);

                //Write the actual image data ((w * h * bpp) bytes)
                fwrite(&surface->pixels, 1, width * height * surface->format->BytesPerPixel, file);

                //Log this sprite as written
                sprite_set.insert(cursor->sprite->name);
            }

            //Also have a place for saving audio (once that's implemented in the system)
            if(audio_set.find(std::string(cursor->sound->name)) == audio_set.end()){

                //Identifier len
                uint16_t identifier_len = strlen(cursor->sound->name);
                uint16_t identifier_len_swapped = EndianSwap(&identifier_len);
                fwrite(&identifier_len_swapped, 2, 1, file);

                //Identifier
                fwrite(cursor->sound->name, 1, identifier_len, file);

                //Writing the audio buffer (from the mixer chunk) to file
                uint32_t audio_len = EndianSwap(&cursor->sound->sample->alen);
                fwrite(&audio_len, 1, 1, file);
                fwrite(&cursor->sound->sample->abuf, 1, cursor->sound->sample->alen, file);

                //Insert the audio
                audio_set.insert(cursor->sound->name);
            }

            cursor = cursor->next;
            sprite_set.insert(cursor->sprite->name);
        } while(cursor != sequence_start);
    }

    return 0;
}