#include "./Entity.hpp"
#include "../Zone/Zone.hpp"
#include "../Engine/Engine.hpp"

/** Entity parameterized constructor
 * @param name The name of the entity
 * @param draw_layer The draw layer of the entity
 */
Entity::Entity(const char* name, int draw_layer){
    this->name = StrDeepCopy(name);

    this->draw_layer = draw_layer;

    //Initializing animation/visibility attributes
    this->active = true;
    this->visible = true;
    this->active_animation = nullptr;

    //Initializing attributes
    this->attr = new AttributeHash(64);
}

/** Construct the entity from file
 * @param file The file you wish to construct the entity with, pointing to the object type indicator
 */
Entity::Entity(FILE* file){
    if(this->deserializeData(file)){
        if(this->name != nullptr){
            printf("Extended object deserialization of %s failed\n", this->name);
        }
        else{
            printf("Extended object deserialization failed\n");
        }
    }
}

/** Destructor for entities
 */
Entity::~Entity(){
    for(auto& animation : animations){
        delete animation.second;
    }
    animations.clear();
    free(name);
}

/** Gets the name of the entity
 * @return The name of the entity
 */
char* Entity::getName(){
    return this->name;
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
double Entity::getUpperDrawAxis(){
    if(this->active_animation == nullptr){
        return 0;
    }
    return this->active_animation->getUpperDrawAxis();
}

/** Gets the draw axis of the entity
 * @return The draw axis of the entity
 */
double Entity::getLowerDrawAxis(){
    if(this->active_animation == nullptr){
        return 0;
    }
    return this->active_animation->getLowerDrawAxis();
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

//Should this be a pure virtual?
//Should AnimationList be an unordered map?
/** Creates a new animation (you MUST do this before adding hitboxes/sprites)
 * @param animation_name The name of the new animation
 * @param num_sprite_sets The number of sprite sets
 */
int Entity::addAnimation(const char* animation_name, uint32_t num_sprite_sets){
    //If the animation already exists, just return
    if(findAnimation(animation_name) != nullptr){
        return -1;
    }

    Animation* new_animation = new Animation(animation_name, &x, &y, num_sprite_sets);
    animations[std::string(animation_name)] = new_animation;
    return 0;
}

/** Creates a new animation (you MUST do this before adding hitboxes/sprites)
 * @param animation A pointer to the new animation
 */
int Entity::addAnimation(Animation* animation){
    std::string animation_name(animation->getName());
    if(animations.find(animation_name) != animations.end()){
        return -1;
    }

    animations[animation_name] = animation;
    return 0;
}

/** Adds a sprite to a given animation
 * @param animation_name The animation name
 * @param sprite_path The filepath to the sprite you're adding
 * @param keytime The number of frames until the animation progresses
 * @param iter The number of frames you'd like to add
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

/** Adds a sprite for a given animation in the entity
 * @param animation_name The animation set you'd like to add the sprite to
 * @param sprite_set The sprite set you'd like to add the sprite to
 * @param sprite_id The sprite ID
 * @param x_offset The X offset of the sprite
 * @param y_offset The Y offset of the sprite
 * @param width The width of the sprite (or -1 if it should be calculated via image size)
 * @param height The height of the sprite (or -1 if it should be calculated via the image size)
 * @return -1 if adding the sprite failed, 0 otherwise
 */
int Entity::addSprite(const char* animation_name, const char* sprite_set, const char* sprite_id, int x_offset, int y_offset, int width, int height){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }

    //If the animation doesn't have the sprite set, add it
    if(!animation->hasSpriteSet(sprite_set)){
        animation->addSpriteSet(sprite_set);
    }

    return animation->addSprite(sprite_set, sprite_id, x_offset, y_offset, width, height);
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
    for(auto& animation : animations){
        ret |= animation.second->setSpriteSet(sprite_set);
    }

    return ret;
}

/** Sets the draw axis for the specified animation in the entity
 * @param animation_name The name of the animation you'd like to set the draw axis of
 * @param draw_axis The draw axis (offset from the the top of the sprite)
 * @param sprite_num The sprite number you'd like to set the upper draw axis for (-1 is all sprites in the animation)
 */
int Entity::setUpperDrawAxis(const char* animation_name, double draw_axis, int32_t sprite_num){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }

    animation->setUpperDrawAxis(draw_axis, sprite_num);
    return 0;
}

/** Sets the draw axis for the specified animation in the entity
 * @param animation_name The name of the animation you'd like to set the draw axis of
 * @param draw_axis The draw axis (offset from the the top of the sprite)
 * @param sprite_num The sprite number you'd like to set the lower draw axis for (-1 is all sprites in the animation)
 */
int Entity::setLowerDrawAxis(const char* animation_name, double draw_axis, int32_t sprite_num){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }

    animation->setLowerDrawAxis(draw_axis, sprite_num);
    return 0;
}

/** Sets the draw axis for all animations in the entity
 * @param draw_axis The draw axis (offset from the the top of the sprite)
 * @param sprite_num The sprite number you'd like to set the upper draw axis for (-1 is all sprites in the animation)
 */
int Entity::setUpperDrawAxis(double draw_axis, int32_t sprite_num){
    if(animations.empty()){
        return -1;
    }

    for(auto& animation : animations){
        animation.second->setUpperDrawAxis(draw_axis, sprite_num);
    }

    return 0;
}

/** Sets the draw axis for all animations in the entity
 * @param draw_axis The draw axis (offset from the the top of the sprite)
 * @param sprite_num The sprite number you'd like to set the lower draw axis for (-1 is all sprites in the animation)
 */
int Entity::setLowerDrawAxis(double draw_axis, int32_t sprite_num){
    if(animations.empty()){
        return -1;
    }

    for(auto& animation : animations){
        animation.second->setLowerDrawAxis(draw_axis, sprite_num);
    }

    return 0;
}

/** Adds a sound to a given animation
 * @param animation_name The animation name
 * @param sound_id The identifier of the sound you're adding
 * @param sequence_num The sequence number you'd like to add the sound to
 * @return 0 on success, -1 if the animation doesn't exist
 */
int Entity::addSound(const char* animation_name, const char* sound_id, int sequence_num){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }
    return animation->addSound(sound_id, sequence_num);
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
    for(auto& animation : animations){
        animation.second->setSize(width, height);
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

/** Sets the attribute key to the specified (as a bool)
 * @param key The key you wish to set
 * @param val The val you wish to set
 */
void Entity::setAttr(const char* key, bool val){
    this->attr->set(key, (void*)val, ATTR_DATA_TYPE::DATA_BOOL);
}

/** Sets the attribute key to the specified val (as a char)
 * @param key The key you wish to set
 * @param val The val you wish to set
 */
void Entity::setAttr(const char* key, char val){
    this->attr->set(key, (void*)val, ATTR_DATA_TYPE::DATA_CHAR);
}

/** Sets the attribute key to the specified val (as a signed int)
 * @param key The key you wish to set
 * @param val The val you wish to set
 */
void Entity::setAttr(const char* key, int64_t val){
    this->attr->set(key, (void*)val, ATTR_DATA_TYPE::DATA_INT);
}

/** Sets the attribute key to the specified val (as an unsigned int)
 * @param key The key you wish to set
 * @param val The val you wish to set
 */
void Entity::setAttr(const char* key, uint64_t val){
    this->attr->set(key, (void*)val, ATTR_DATA_TYPE::DATA_UINT);
}

/** Sets the attribute key to the specified val (as a string)
 * @param key The key you wish to set
 * @param val The val you wish to set
 */
void Entity::setAttr(const char* key, char* val){
    this->attr->set(key, (void*)val, ATTR_DATA_TYPE::DATA_STRING);
}

/** Gets the entity type
 * @return A uint32_t representation of the entity type set in the constructor
 */
uint32_t Entity::getType(){
    return this->type;
}

/** Finds an animation within the entity
 * @param animation_name The name of the animation
 * @return The animation pointer, or nullptr if it's not found
 */
Animation* Entity::findAnimation(const char* animation_name){
    std::string animation_str(animation_name);
    if(animations.find(animation_str) == animations.end()){
        return nullptr;
    }
    return animations[animation_str];
}

/** Serializes data
 * @param file The file to write serialization data to
 * @param base_zone The base zone that the entity belongs to (needed for using the local object offset)
 * @return -1 if serialization failed, 0 otherwise
 */
int Entity::serializeData(FILE* file, Zone* base_zone){
    if(file == nullptr){
        return -1;
    }

    //GENERAL SECTION
    //Write the entity type
    WriteVar(this->type, uint32_t, file);

    //Identifier
    uint16_t identifier_len = strlen(this->name);
    WriteVar(identifier_len, uint16_t, file);
    fwrite(this->name, 1, identifier_len, file);

    //Draw layer
    WriteVar(draw_layer, int16_t, file);

    //ATTRIBUTE SECTION
    Attribute* attr_list;
    uint32_t num_entries = attr->dump(&attr_list);
    WriteVar(num_entries, uint32_t, file);

    while(attr_list != nullptr){
        //Attribute type
        uint16_t attr_type = attr_list->type;
        WriteVar(attr_type, uint16_t, file);

        //Write the key info
        uint16_t attr_key_len = strlen(attr_list->key);
        WriteVar(attr_key_len, uint16_t, file);
        fwrite(attr_list->key, attr_key_len, 1, file);

        //Write the val info
        switch(attr_type){
            //Writing chars & bools (1 byte)
            case ATTR_DATA_TYPE::DATA_BOOL:
            case ATTR_DATA_TYPE::DATA_CHAR:{
                uint8_t attr_val = (uint8_t)((uint64_t)attr_list->val & 0xFF);
                WriteVar(attr_val, uint8_t, file);
                break;
            }

            //Writing ints/uints (8 bytes)
            case ATTR_DATA_TYPE::DATA_INT:
            case ATTR_DATA_TYPE::DATA_UINT:{
                WriteVar((uint64_t)attr_list->val, uint64_t, file);
                break;
            }

            //Writing strings (? bytes)
            case ATTR_DATA_TYPE::DATA_STRING:{
                char* attr_str = (char*)attr_list->val;
                uint16_t attr_val_len = strlen(attr_str);
                WriteVar(attr_val_len, uint16_t, file);
                fwrite(attr_str, 1, attr_val_len, file);
                break;
            }
        }
    }

    //ANIMATION SECTION
    WriteVar(animations.size(), uint16_t, file);

    for(auto& animation : animations){
        animation.second->serializeData(file);
    }

    if(this->active_animation != nullptr){
        const char* starting_animation = this->active_animation->getName();
        uint16_t starting_animation_len = strlen(starting_animation);
        WriteVar(starting_animation_len, uint16_t, file);
        fwrite(starting_animation, 1, starting_animation_len, file);
    }
    else{
        Animation* default_animation = this->animations.begin()->second;
        if(default_animation != nullptr){
            const char* starting_animation = default_animation->getName();
            uint16_t starting_animation_len = strlen(starting_animation);
            WriteVar(starting_animation_len, uint16_t, file);
            fwrite(starting_animation, 1, starting_animation_len, file);
        }
        else{
            const uint16_t zero = 0;
            fwrite(&zero, sizeof(zero), 1, file);
        }
    }

    return 0;
}

int Entity::deserializeData(FILE* file){
    if(file == nullptr){
        return -1;
    }

    //Entity type
    ReadVar(this->type, file);

    //Identifier
    uint16_t identifier_len;
    ReadVar(identifier_len, file);
    this->name = (char*)malloc(identifier_len + 1);
    fread(&this->name, sizeof(char), identifier_len, file);
    name[identifier_len] = '\0';

    //Draw layer
    ReadVar(this->draw_layer, file);

    //ATTRIBUTE SECTION
    uint32_t num_entries;
    ReadVar(num_entries, file);

    for(uint32_t i = 0; i < num_entries; i++){
        //Attribute type
        uint16_t attr_type;
        ReadVar(attr_type, file);

        //Attribute key
        uint16_t attr_key_len;
        ReadVar(attr_key_len, file);
        char attr_key[attr_key_len + 1];
        fread(attr_key, attr_key_len, 1, file);
        attr_key[attr_key_len] = '\0';

        switch(attr_type){
            //Reading chars & bools (1 byte)
            case ATTR_DATA_TYPE::DATA_BOOL:{
                bool attr_val;
                ReadVar(attr_val, file);
                this->setAttr(attr_key, attr_val);
                break;
            }
            case ATTR_DATA_TYPE::DATA_CHAR:{
                char attr_val;
                ReadVar(attr_val, file);
                this->setAttr(attr_key, attr_val);
                break;
            }

            //Reading ints/uints (8 bytes)
            case ATTR_DATA_TYPE::DATA_INT:{
                int64_t attr_val;
                ReadVar(attr_val, file);
                this->setAttr(attr_key, attr_val);
                break;
            }
            case ATTR_DATA_TYPE::DATA_UINT:{
                uint64_t attr_val;
                ReadVar(attr_val, file);
                this->setAttr(attr_key, attr_val);
                break;
            }

            //Reading strings (? bytes)
            case ATTR_DATA_TYPE::DATA_STRING:{
                uint16_t attr_str_len;
                ReadVar(attr_str_len, file);
                char attr_str[attr_str_len + 1];
                fread(attr_str, 1, attr_str_len, file);
                attr_str[attr_str_len] = '\0';
                this->setAttr(attr_key, attr_str);
                break;
            }
        }
    }

    //ANIMATION SECTION
    uint16_t num_animations;
    ReadVar(num_animations, file);
    for(int i = 0; i < num_animations; i++){
        Animation* new_animation = new Animation(file);
        this->addAnimation(new_animation);
    }

    //Set staring animation
    uint16_t starting_str_len;
    ReadVar(starting_str_len, file);
    if(starting_str_len > 0){
        char starting_str[starting_str_len + 1];
        fread(starting_str, starting_str_len, 1, file);
        starting_str[starting_str_len] = '\0';
        this->setAnimation(starting_str);
    }

    return 0;
}

/** Saves the resources of the entity to a char*'s (which should be freed upon return)
 * @param file The pointer to the open file to write to
 * @param written_sprites The set of sprites that have already been written to file
 * @param written_audio The set of audio assets that have already been written to file
 * @param written_music The set of music assets that have already been written to file (used just by entities that handle music)
 * @return The number of bytes that *buff_ptr is
 */
int Entity::serializeAssets(FILE* file, SerializeSet& serialize_set){
    if(file == nullptr){
        return -1;
    }

    for(auto& animation : animations){
        animation.second->serializeAssets(file, serialize_set);
    }

    return 0;
}