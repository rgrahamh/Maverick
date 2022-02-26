#include "Loader.hpp"
#include "../../../Maverick/Utility/Utility.hpp"
#include "../../../Maverick/Engine/Engine.hpp"

extern Engine* engine;

/** Loads a BMP from file & inserts it into the engine
 * @param file The file to load the BMP from
 */
inline void loadBMP(FILE* file){
	uint8_t depth, bytes_per_pixel;
	uint16_t identifier_len;
	uint32_t width, height, rmask, gmask, bmask, amask;

	if(fread(&identifier_len, sizeof(identifier_len), 1, file) == 0){
		printf("Cannot read in vals!\n");
	}
	identifier_len = EndianSwap(&identifier_len);

	char identifier[identifier_len+1];
	fread(&identifier, 1, identifier_len, file);
	identifier[identifier_len] = '\0';

	fread(&width, sizeof(width), 1, file);
	fread(&height, sizeof(height), 1, file);
	width = EndianSwap(&width);
	height = EndianSwap(&height);

	fread(&depth, sizeof(depth), 1, file);

	fread(&rmask, sizeof(rmask), 1, file);
	fread(&gmask, sizeof(gmask), 1, file);
	fread(&bmask, sizeof(bmask), 1, file);
	fread(&amask, sizeof(amask), 1, file);

	fread(&bytes_per_pixel, sizeof(bytes_per_pixel), 1, file);

	rmask = EndianSwap(&rmask);
	gmask = EndianSwap(&gmask);
	bmask = EndianSwap(&bmask);
	amask = EndianSwap(&amask);

	SDL_Surface* new_surface = SDL_CreateRGBSurface(0, width, height, depth, rmask, gmask, bmask, amask);

	uint64_t byte_size = width * height * bytes_per_pixel;
	new_surface->pixels = (uint8_t*)malloc(byte_size);

	fread(new_surface->pixels, 1, byte_size, file);

	engine->addSurface(identifier, new_surface);
}


/** Loads a sound from file & inserts it into the engine
 * @param file The file to load the sound from
 */
inline void loadSound(FILE* file){
	uint16_t identifier_len;

	fread(&identifier_len, sizeof(identifier_len), 1, file);
	identifier_len = EndianSwap(&identifier_len);

	char* identifier = (char*)malloc(identifier_len+1);
	fread(&identifier, 1, identifier_len, file);
	identifier[identifier_len] = '\0';

	Sound* new_sound = new Sound;
	new_sound->name = identifier;

	uint32_t audio_len;
	fread(&audio_len, sizeof(audio_len), 1, file);
	audio_len = EndianSwap(&audio_len);
	new_sound->sample->alen = audio_len;

	fread(&new_sound->sample->abuf, 1, audio_len, file);

	engine->addSound(identifier, new_sound);
}

/** Loads a unit of music from file & inserts it into the engine
 * @param file The file to load the music from
 */
inline void loadMusic(FILE* file){
	return;
}

inline void loadCutscene(FILE* file){
	return;
}

int loadAssets(const char* zone_name)
{
	int zone_name_len = strlen(zone_name);

	char* pad_name = (char*)calloc(zone_name_len + 5, 1);
	memcpy(pad_name, zone_name, zone_name_len);
	strcat(pad_name, ".pad");

	FILE* file = fopen(pad_name, "rb");

	if(file == nullptr){
		printf("Could not open %s for reading!\n", pad_name);
		return -1;
	}

	uint8_t type;
	while(fread(&type, 1, sizeof(type), file) == 1){
		switch(type){
			case(RESOURCE_TYPE::BMP):
				loadBMP(file);
				break;
			case(RESOURCE_TYPE::SOUND):{
				loadSound(file);
				break;
			}
			//Implement later
			case(RESOURCE_TYPE::MUSIC):
				loadMusic(file);
				break;
			//Implement later
			case(RESOURCE_TYPE::CUT):
				loadCutscene(file);
				break;
		}
	}

	fclose(file);

	return 0;
}

Zone* loadData(const char* zone_name){
	int zone_name_len = strlen(zone_name);

	char* name = (char*)calloc(zone_name_len + 5, 1);
	memcpy(name, zone_name, zone_name_len);
	strcat(name, ".pld");

	FILE* file = fopen(name, "rb");
	if(file == nullptr){
		printf("Cannot open %s for reading!\n", zone_name);
		return nullptr;
	}

	int64_t global_x_coord, global_y_coord;
	fread(&global_x_coord, sizeof(global_x_coord), 1, file);
	fread(&global_y_coord, sizeof(global_y_coord), 1, file);

	global_x_coord = EndianSwap(&global_x_coord);
	global_y_coord = EndianSwap(&global_y_coord);

	Zone* zone = new Zone(zone_name, global_x_coord, global_y_coord);

	fclose(file);

	return zone;
}

Zone* loadZoneFromFile(const char* zone_name){
    if(zone_name == nullptr){
		printf("Can't load zone; zone_name is null!\n");
		return nullptr;
    }

	if(loadAssets(zone_name)){
		return nullptr;
	}

	return loadData(zone_name);
}