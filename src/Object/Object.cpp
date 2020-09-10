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
    /*int hitbox_num = sizeof(this->hitboxes)/sizeof(Hitbox*);
    for(int i = 0; i < hitbox_num; i++){
        if(hitboxes[i] != NULL){
            free(hitboxes[i]);
        }
    }
    free(hitboxes);*/

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

void Object::action(sf::Event){

}

void Object::_process(){
    //Updating X values
    this->xV = this->xA + (this->xV * (1 - this->friction));
    this->x += this->xV;
    this->xA = 0;

    //Updating Y values
    this->yV = this->yA + (this->yV * (1 - this->friction));
    this->y += this->yV;
    this->yV = 0;

    this->process();
}

void Object::process(){
}

void Object::draw(sf::RenderWindow* window){
    this->animations[active_animation]->draw(window);
}