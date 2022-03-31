#include "Saver.hpp"

int saveZone(Zone* zone){
    if(zone == nullptr){
		printf("Zone ptr is null\n");
		return -1;
	}

	const char* zone_name = zone->getName();
	int zone_name_len = strlen(zone_name);
	char* zone_file_name = (char*)calloc(zone_name_len + 5, 1);
	memcpy(zone_file_name, zone_name, zone_name_len);
	strcat(zone_file_name, ".pld");

	char* asset_file_name = (char*)calloc(zone_name_len + 5, 1);
	memcpy(asset_file_name, zone_name, zone_name_len);
	strcat(asset_file_name, ".pad");

	//Open the file
	FILE* asset_file = fopen(asset_file_name, "wb");
	FILE* zone_file = fopen(zone_file_name, "wb");
	if(zone_file == nullptr || asset_file == nullptr){
		printf("Can't open files for saving!\n");
		return -1;
	}

	ObjectList* objects = zone->getObjects();
	UIElementList* elements = zone->getUIElements();

	SerializeSet serialize_set;

	//Save resources used in this zone (both by UI elements & objects)
	while(elements != nullptr && elements->element != nullptr){
		UIElement* element = elements->element;
		element->serializeAssets(asset_file, serialize_set);
		element->serializeData(zone_file, zone);

		elements = elements->next;
	}
	while(objects != nullptr && objects->obj != nullptr){
		Object* obj = objects->obj;
		obj->serializeAssets(asset_file, serialize_set);
		obj->serializeData(zone_file, zone);

		objects = objects->next;
	}

	fclose(zone_file);
	fclose(asset_file);

	return 0;
}