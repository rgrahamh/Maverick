#include "./Object.hpp"

Object::Object(float start_x, float start_y, unsigned int draw_layer){
    this->x = start_x;
    this->y = start_y;
    this->draw_layer = draw_layer;
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

void Object::_draw(sf::RenderWindow* window, unsigned int draw_layer){
    this->animation->advance();
    if(this->draw_layer == draw_layer){
        window->draw(*(this->animation->getSprite()));
        this->draw(window);
    }
}

void Object::draw(sf::RenderWindow* window){
}