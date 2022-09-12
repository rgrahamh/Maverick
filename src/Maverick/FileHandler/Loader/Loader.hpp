//What the color of her eyes were, or her scars and how she got them

#ifndef LOADER_H
#define LOADER_H
#include "Maverick/Zone/Zone.hpp"
#include "Maverick/Font/Font.hpp"
#include "Maverick/Audio/Music/Music.hpp"
#include "Maverick/Global/Global.hpp"

Zone* loadZoneFromFile(const char* zone_name);

SDL_Surface* loadBMP(FILE* file, char* key_buff = nullptr, unsigned int max_len = 0);
Sound* loadSound(FILE* file, char* key_buff = nullptr, unsigned int max_len = 0);
Music* loadMusic(FILE* file, char* key_buff = nullptr, unsigned int max_len = 0);
Font* loadFont(FILE* file, char* key_buff = nullptr, unsigned int max_len = 0);
#endif