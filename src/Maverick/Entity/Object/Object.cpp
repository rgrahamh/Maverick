#include "./Object.hpp"
#include "../../Zone/Zone.hpp"
#include "../../Engine/Engine.hpp"

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

Object::~Object(){
}

double Object::getOldX(){
    return this->old_x;
}

double Object::getOldY(){
    return this->old_y;
}

double Object::getOldZ(){
    return this->old_z;
}

double Object::getXVel(){
    return this->xV;
}

double Object::getYVel(){
    return this->yV;
}

double Object::getZVel(){
    return this->zV;
}

double Object::getX(){
    return this->x;
}

double Object::getY(){
    return this->y;
}

double Object::getZ(){
    return this->z;
}

double Object::getWidth(){
    if(this->active_animation == nullptr){
        return 0;
    }
    Sprite* sprite = this->active_animation->getSprite();
    if(sprite == nullptr){
        return 0;
    }
    return sprite->rect->w;
}

double Object::getHeight(){
    if(this->active_animation == nullptr){
        return 0;
    }
    Sprite* sprite = this->active_animation->getSprite();
    if(sprite == nullptr){
        return 0;
    }
    return sprite->rect->h;
}

float Object::getMass(){
    return this->mass;
}

bool Object::getEnvBump(){
    return this->env_bump;
}

Sprite* Object::getSprite(){
    if(this->active_animation != nullptr){
        return this->active_animation->getSprite();
    }
    else{
        return nullptr;
    }
}

int Object::getCollisionLayer(){
    return this->collision_layer;
}

float Object::getTerminalVelocity(){
    return this->terminal_velocity;
}

double Object::getGround(){
    return this->ground;
}

double Object::getNextGround(){
    return this->next_ground;
}

void Object::setXVel(double xV){
    this->xV = xV;
}

void Object::setYVel(double yV){
    this->yV = yV;
}

void Object::setZVel(double zV){
    this->zV = zV;
}

void Object::setX(double x){
    this->x = x;
}

void Object::setY(double y){
    this->y = y;
}

void Object::setZ(double z){
    this->z = z;
}

void Object::setFriction(float friction){
    this->friction = friction;
}

void Object::setEnvBump(){
    this->env_bump = true;
}

void Object::setCollisionLayer(int collision_layer){
    this->collision_layer = collision_layer;
}

void Object::setGround(double ground){
    this->ground = ground;
}

void Object::setNextGround(double next_ground){
    this->next_ground = next_ground;
}

void Object::applyForce(double xA, double yA, double zA){
    this->xA += xA / this->mass;
    this->yA += yA / this->mass;
    this->zA += zA / this->mass;
}

void Object::_action(Control* control){
    this->action(control);
}

void Object::action(Control* control){
    return;
}

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

void Object::process(uint64_t delta, unsigned int steps){
}

void Object::_draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y){
    this->draw(renderer, delta, camera_x, camera_y);
}

void Object::draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y){
    if(this->active_animation != nullptr){
        this->active_animation->draw(renderer, delta, camera_x, camera_y, this->z);
    }
}

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

HitboxList* Object::getHitboxes(){
    if(this->active_animation == nullptr){
        return nullptr;
    }
    return this->active_animation->getHitboxes();
}

void Object::onCollide(Object* other, Hitbox* this_hitbox, Hitbox* other_hitbox){
}

Animation* Object::findAnimation(const char* animation_name){
    std::string animation_str(animation_name);
    if(animations.find(animation_str) == animations.end()){
        return nullptr;
    }
    return animations[animation_str];
}

int Object::addAnimation(const char* animation_name, uint32_t num_sprite_sets){
    //If the animation already exists, just return
    if(findAnimation(animation_name) != nullptr){
        return -1;
    }

    Animation* new_animation = new Animation(animation_name, &x, &y, num_sprite_sets);
    animations[std::string(animation_name)] = new_animation;
    return 0;
}

int Object::addAnimation(Animation* animation){
    std::string animation_name(animation->getName());
    if(animations.find(animation_name) != animations.end()){
        return -1;
    }

    animations[animation_name] = animation;
    return 0;
}

int Object::addFrame(const char* animation_name, unsigned int keytime, unsigned int iter){
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

int Object::addSprite(const char* animation_name, const char* sprite_set, const char* sprite_id, int x_offset, int y_offset, int width, int height){
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

int Object::addSpriteSet(const char* animation_name, const char* sprite_set){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }

    return animation->addSpriteSet(sprite_set);
}

int Object::setSpriteSet(const char* animation_name, const char* sprite_set){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }

    return animation->setSpriteSet(sprite_set);
}

int Object::setSpriteSet(const char* sprite_set){
    int ret = 0;
    for(auto& animation : animations){
        ret |= animation.second->setSpriteSet(sprite_set);
    }

    return ret;
}

double Object::getUpperDrawAxis(){
    if(this->active_animation == nullptr){
        return 0;
    }
    return this->active_animation->getUpperDrawAxis();
}

double Object::getLowerDrawAxis(){
    if(this->active_animation == nullptr){
        return 0;
    }
    return this->active_animation->getLowerDrawAxis();
}

int Object::setUpperDrawAxis(const char* animation_name, double draw_axis, int32_t sprite_num){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }

    animation->setUpperDrawAxis(draw_axis, sprite_num);
    return 0;
}

int Object::setLowerDrawAxis(const char* animation_name, double draw_axis, int32_t sprite_num){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }

    animation->setLowerDrawAxis(draw_axis, sprite_num);
    return 0;
}

int Object::setUpperDrawAxis(double draw_axis, int32_t sprite_num){
    if(animations.empty()){
        return -1;
    }

    for(auto& animation : animations){
        animation.second->setUpperDrawAxis(draw_axis, sprite_num);
    }

    return 0;
}

int Object::setLowerDrawAxis(double draw_axis, int32_t sprite_num){
    if(animations.empty()){
        return -1;
    }

    for(auto& animation : animations){
        animation.second->setLowerDrawAxis(draw_axis, sprite_num);
    }

    return 0;
}

int Object::addSound(const char* animation_name, const char* sound_id, int sequence_num){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }
    return animation->addSound(sound_id, sequence_num);
}

int Object::setAnimation(const char* animation_name){
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

int Object::setSize(const char* animation_name, float width, float height){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }
    animation->setSize(width, height);
    return 0;
}

void Object::setSize(float width, float height){
    for(auto& animation : animations){
        animation.second->setSize(width, height);
    }
}


int Object::serializeData(FILE* file, Zone* base_zone){
    if(Entity::serializeData(file, base_zone) == -1){
        return 0;
    }

    //Write X/Y offsets from the zone origin
    double write_x = this->x - base_zone->getGlobalX();
    double write_y = this->y - base_zone->getGlobalY();

    WriteVar((uint64_t)write_x, uint64_t, file);
    WriteVar((uint64_t)write_y, uint64_t, file);

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


int Object::deserializeData(FILE* file){
    if(Entity::deserializeData(file) != 0){
        return -1;
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
}

int Object::serializeAssets(FILE* file, SerializeSet& serialize_set){
    int ret = 0;
    ret |= Entity::serializeAssets(file, serialize_set);
    
    for(auto& animation : animations){
        ret |= animation.second->serializeAssets(file, serialize_set);
    }

    return ret;
}