//What the color of her eyes were, or her scars and how she got them

#ifndef LOADER_H
#define LOADER_H
#include "../../Zone/Zone.hpp"
#include "../../Font/Font.hpp"
#include "../../../Maverick/Global/Global.hpp"

Zone* loadZoneFromFile(const char* zone_name);

char* loadBMP(FILE* file);
char* loadSound(FILE* file);
char* loadMusic(FILE* file);
char* loadFont(FILE* file);
#endif