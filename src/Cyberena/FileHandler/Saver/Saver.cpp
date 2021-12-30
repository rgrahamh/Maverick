#include "Saver.hpp"

int saveZone(Zone* zone){
	bool endian = getEndian();

    if(zone == nullptr){
		printf("Zone ptr is null\n");
		return -1;
	}

	const char* zone_name = zone->getName();
	int zone_name_len = strlen(zone_name);
	char* zone_file_name = (char*)calloc(zone_name_len + 5, 1);
	memcpy(zone_file_name, zone_name, zone_name_len);
	strcat(zone_file_name, ".pld");

	//Open the file
	FILE* zone_file = fopen(zone_file_name, "w");
	if(zone_file == nullptr){
		printf("Can't open %s for saving!\n", zone_file_name);
		return -1;
	}

	ObjectList* objects = zone->getObjects();
	UIElementList* elements = zone->getUIElements();
	ObjectList* obj_cursor = objects;
	UIElementList* element_cursor = elements;

	std::unordered_set<std::string> sprite_set;
	std::unordered_set<std::string> audio_set;

	//Save resources used in this zone (both by UI elements & objects)
	while(element_cursor != nullptr && element_cursor->element != nullptr){
		UIElement* element = element_cursor->element;
		char* assets_buff = nullptr;
		unsigned int buff_len = element->serializeAssets(&assets_buff, sprite_set, audio_set);
		if(buff_len > 0 && assets_buff != nullptr){
			fwrite(assets_buff, buff_len, 1, zone_file);
			delete assets_buff;
		}

		element_cursor = element_cursor->next;
	}
	while(obj_cursor != nullptr && obj_cursor->obj != nullptr){
		Object* obj = obj_cursor->obj;
		char* assets_buff = nullptr;
		unsigned int buff_len = obj->serializeAssets(&assets_buff, sprite_set, audio_set);
		if(buff_len > 0 && assets_buff != nullptr){
			fwrite(assets_buff, buff_len, 1, zone_file);
			delete assets_buff;
		}	

		obj_cursor = obj_cursor->next;
	}

	//Rewind cursors
	obj_cursor = objects;
	element_cursor = elements;

	//Write UI elements to file
	while(element_cursor != nullptr && element_cursor->element != nullptr){
		UIElement* element = element_cursor->element;
		char* data_buff = nullptr;
		unsigned int buff_len = element->serializeData(&data_buff);
		if(buff_len > 0 && data_buff != nullptr){
			fwrite(data_buff, buff_len, 1, zone_file);
			delete data_buff;
		}

		element_cursor = element_cursor->next;
	}

	//Write objects to file
	while(obj_cursor != nullptr && obj_cursor->obj != nullptr){
		Object* obj = obj_cursor->obj;
		char* data_buff = nullptr;
		unsigned int buff_len = obj->serializeData(&data_buff);
		if(buff_len > 0 && data_buff != nullptr){
			fwrite(data_buff, buff_len, 1, zone_file);
			delete data_buff;
		}	

		obj_cursor = obj_cursor->next;
	}

	fclose(zone_file);

	return 0;
}