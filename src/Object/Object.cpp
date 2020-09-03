#include "./Object.hpp"

/** Object parameterized constructor
 * @param start_x The starting X location of the object
 * @param start_y The starting Y location of the objcet
 * @param draw_layer The draw layer of the object
 * @param animation_num The number of animations
 */
Object::Object(float start_x, float start_y, unsigned int draw_layer, unsigned int animation_num){
    this->x = start_x;
    this->y = start_y;
    this->draw_layer = draw_layer;
    this->animations = (Animation**)calloc(sizeof(Animation*), animation_num);
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
    return this->width;
}

/** Gets the height of the object
 * @return The height of the object
 */
float Object::getHeight(){
    return this->height;
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

/** Draws the object
 * @param window The window to draw to
 */
void Object::_draw(sf::RenderWindow* window){
    this->animations[active_animation]->advance();
    
    window->draw(*(this->animations[active_animation]->getSprite()));
    this->draw(window);
}

void Object::draw(sf::RenderWindow* window){
}