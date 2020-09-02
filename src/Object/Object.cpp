#include "./Object.hpp"

Object::Object(float start_x, float start_y, unsigned int draw_layer, unsigned int animation_num){
    this->x = start_x;
    this->y = start_y;
    this->draw_layer = draw_layer;
    this->animations = (Animation**)calloc(sizeof(Animation*), animation_num);
}

float Object::getX(){
    return this->x;
}

float Object::getY(){
    return this->y;
}

float Object::getWidth(){
    return this->width;
}

float Object::getHeight(){
    return this->height;
}

int Object::getDrawLayer(){
    return this->draw_layer;
}

void Object::addSprite(unsigned int animation_num, const char* sprite_path, unsigned int keyframe, float x_offset, float y_offset){

}

void Object::_draw(sf::RenderWindow* window, unsigned int draw_layer){
    this->animations[active_animation]->advance();
    
    if(this->draw_layer == draw_layer){
        window->draw(*(this->animations[active_animation]->getSprite()));
        this->draw(window);
    }
}

void Object::draw(sf::RenderWindow* window){
}