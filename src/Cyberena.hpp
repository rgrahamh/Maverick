#include <SFML/Graphics.hpp>
#include "./Engine/Engine.hpp"

Engine* engine;

Zone* initZones(unsigned char zone_num);

ZoneLst* zones;

float X_SCALE = sf::VideoMode::getDesktopMode().width;
float Y_SCALE = sf::VideoMode::getDesktopMode().height;
