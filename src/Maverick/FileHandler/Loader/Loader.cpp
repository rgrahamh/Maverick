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

void loadAssets(const char* zone_name)
{
	int zone_name_len = strlen(zone_name);

	char* pad_name = (char*)calloc(zone_name_len + 5, 1);
	memcpy(pad_name, zone_name, zone_name_len);
	strcat(pad_name, ".pad");

	FILE* file = fopen(pad_name, "rb");

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
}

void loadData(FILE* file){
	return;
}

Zone* loadZoneFromFile(const char* zone_name){
    if(zone_name == nullptr){
		printf("Can't load zone; zone_name is null!\n");
		return nullptr;
    }

	loadAssets(zone_name);

	int zone_name_len = strlen(zone_name);

	char* pld_name = (char*)calloc(zone_name_len + 5, 1);
	memcpy(pld_name, zone_name, zone_name_len);
	strcat(pld_name, ".pld");

	FILE* pld_file = fopen(pld_name, "rb");

	fclose(pld_file);

	/*//Load global coordinates
	int64_t global_x_coord = EndianSwap((int32_t*)cursor);
	cursor += 8;
	int64_t global_y_coord = EndianSwap((int32_t*)cursor);
	cursor += 8;

	Zone* zone = new Zone(zone_name, global_x_coord, global_y_coord);

	//The resource number
	uint32_t resource_num = EndianSwap((uint32_t*)cursor);
	cursor += 4;

	//The resource block
	for(int i = 0; i < resource_num; i++){
		//The resource type
		uint16_t resource_type = EndianSwap((uint16_t*)cursor);
		cursor += 2;

		//The identifier len
		uint16_t identifier_len = EndianSwap((uint16_t*)cursor);
		cursor += 2;

		//The resource identifier
		char identifier[identifier_len + 1];
		memcpy(identifier, cursor, identifier_len);
		identifier[identifier_len] = '\0';
		cursor += identifier_len;

		//The data len
		uint16_t data_len = EndianSwap((uint16_t*)cursor);
		cursor += 2;

		//The data sector

		if(resource_type == RESOURCE_TYPE::BMP && engine->getSurface(identifier) == nullptr){
			//Image width
			uint32_t width = EndianSwap((uint32_t*)cursor);
			cursor += 4;

			//Image height
			uint32_t height = EndianSwap((uint32_t*)cursor);
			cursor += 4;

			//Color depth
			uint16_t color_depth = EndianSwap((uint16_t*)cursor);
			cursor += 2;
			data_len -= 10;

			SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, color_depth, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);
			
			//Image data
			memcpy(surface->pixels, cursor, data_len);
			cursor += data_len;

			engine->addSurface(identifier, surface);
		}
		else if(resource_type == RESOURCE_TYPE::MUSIC){
			SDL_AudioSpec* spec = new SDL_AudioSpec();
			spec->silence = 0;


			//Frequency
			spec->freq = EndianSwap((uint16_t*)cursor);
			cursor += 2;

			//Channels
			spec->channels = *cursor;
			cursor++;
		}
	}

	fclose(zone_file);*/

	return nullptr;
}