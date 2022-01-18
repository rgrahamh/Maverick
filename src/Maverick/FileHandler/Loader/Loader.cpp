#include "Loader.hpp"
#include "../../../Maverick/Utility/Utility.hpp"
#include "../../../Maverick/Engine/Engine.hpp"

extern Engine* engine;

Zone* loadZoneFromFile(const char* zone_name){
    if(zone_name == nullptr){
		printf("Can't load zone; zone_name is null!\n");
		return nullptr;
    }

	int zone_name_len = strlen(zone_name);
	char* zone_file_name = (char*)calloc(zone_name_len + 5, 1);
	memcpy(zone_file_name, zone_name, zone_name_len);
	strcat(zone_file_name, ".pld");

	FILE* zone_file = fopen(zone_file_name, "rb");
	if(zone_file == nullptr){
		printf("Can't load zone; %s can't be opened!\n", zone_file);
		return nullptr;
	}

	fseek(zone_file, 0, SEEK_END);
	uint32_t file_size = ftell(zone_file);

	char file_buff[file_size];
	char* cursor = file_buff;

	//Load global coordinates
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

	fclose(zone_file);
}