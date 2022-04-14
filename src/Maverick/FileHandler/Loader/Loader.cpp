#include "Loader.hpp"
#include "../../../Maverick/Engine/Engine.hpp"

extern Engine* engine;

inline SDL_Surface* readSurface(FILE* file){
	uint8_t depth, bytes_per_pixel;
	uint32_t width, height, rmask, gmask, bmask, amask;

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
	if(new_surface == nullptr){
		return nullptr;
	}

	uint64_t byte_size = width * height * bytes_per_pixel;
	new_surface->pixels = (uint8_t*)malloc(byte_size);

	fread(new_surface->pixels, 1, byte_size, file);

	return new_surface;
}

/** Loads a BMP from file & inserts it into the engine
 * @param file The file to load the BMP from
 */
inline SDL_Surface* loadBMP(FILE* file, char* key_buff, unsigned int max_len){
	if(file == nullptr){
		return nullptr;
	}

	uint16_t identifier_len;
	fread(&identifier_len, sizeof(identifier_len), 1, file);
	identifier_len = EndianSwap(&identifier_len);

	char identifier[identifier_len+1];
	fread(identifier, 1, identifier_len, file);
	identifier[identifier_len] = '\0';

	SDL_Surface* new_surface = readSurface(file);

	if(key_buff != nullptr){
		strncpy(key_buff, identifier, max_len);
	}

	engine->addSurface(identifier, new_surface);

	return new_surface;
}


/** Loads a sound from file & inserts it into the engine
 * @param file The file to load the sound from
 */
inline Sound* loadSound(FILE* file, char* key_buff, unsigned int max_len){
	uint16_t identifier_len;
	fread(&identifier_len, sizeof(identifier_len), 1, file);
	identifier_len = EndianSwap(&identifier_len);

	char* identifier = (char*)malloc(identifier_len+1);
	fread(identifier, 1, identifier_len, file);
	identifier[identifier_len] = '\0';

	Sound* new_sound = new Sound;
	new_sound->name = identifier;

	uint32_t audio_len;
	fread(&audio_len, sizeof(audio_len), 1, file);
	audio_len = EndianSwap(&audio_len);
	new_sound->sample->alen = audio_len;

	fread(&new_sound->sample->abuf, 1, audio_len, file);

	if(key_buff != nullptr){
		strncpy(key_buff, identifier, max_len);
	}

	engine->addSound(identifier, new_sound);

	return new_sound;
}

/** Loads a unit of music from file & inserts it into the engine
 * @param file The file to load the music from
 */
inline Music* loadMusic(FILE* file, char* key_buff, unsigned int max_len){
	uint16_t identifier_len;
	fread(&identifier_len, sizeof(identifier_len), 1, file);
	identifier_len = EndianSwap(&identifier_len);

	char identifier[identifier_len+1];
	fread(identifier, 1, identifier_len, file);
	identifier[identifier_len] = '\0';

	Music* new_music = new Music(identifier);

	uint16_t num_tracks;
	fread(&num_tracks, sizeof(num_tracks), 1, file);
	num_tracks = EndianSwap(&num_tracks);

	for(int i = 0; i < num_tracks; i++){
		uint16_t track_identifier_len;
		fread(&track_identifier_len, sizeof(track_identifier_len), 1, file);
		track_identifier_len = EndianSwap(&track_identifier_len);

		char track_identifier[track_identifier_len + 1];
		fread(&track_identifier, 1, track_identifier_len, file);
		track_identifier[track_identifier_len] = '\0';

		new_music->addTrack(engine->getSound(track_identifier));
	}

	if(key_buff != nullptr){
		strncpy(key_buff, identifier, max_len);
	}

	engine->addMusic(identifier, new_music);

	return new_music;
}

inline Font* loadFont(FILE* file, char* key_buff, unsigned int max_len){
	uint16_t identifier_len;
	fread(&identifier_len, sizeof(identifier_len), 1, file);
	identifier_len = EndianSwap(&identifier_len);

	char identifier[identifier_len+1];
	fread(identifier, 1, identifier_len, file);
	identifier[identifier_len] = '\0';

	Font* new_font = new Font(identifier);

	uint8_t style_num;
	fread(&style_num, sizeof(style_num), 1, file);

	for(uint8_t i; i < style_num; i++){
		uint8_t style_type, char_num;
		fread(&style_type, sizeof(style_type), 1, file);
		fread(&char_num, sizeof(char_num), 1, file);

		for(uint8_t j; j < char_num; j++){
			char new_char;
			fread(&new_char, sizeof(new_char), 1, file);

			SDL_Surface* new_surface = readSurface(file);
			new_font->setCharacter(new_char, new_surface, (enum FONT_STYLE)style_type);
		}
	}

	if(key_buff != nullptr){
		strncpy(key_buff, identifier, max_len);
	}

	engine->addFont(identifier, new_font);

	return new_font;
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
			case(RESOURCE_TYPE::MUSIC):
				loadMusic(file);
				break;
			case(RESOURCE_TYPE::FONT):
				loadFont(file);
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