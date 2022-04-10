#include "./aed.hpp"
#include "../Maverick/FileHandler/Loader/Loader.hpp"
#include "../Maverick/Engine/Engine.hpp"

extern Engine* engine;

inline int printHelp(char* help_section){
	if(strcmp(help_section, "main_loop") == 0){
		printf("You may use the following options:\n");
		printf("ls: Show resource files in the input directory\n");
		printf("add <resource_type> <name>: Add a resource\n");
		printf("edit <resource_type> <name>: Edit a resource\n");
		printf("rm <resource_type> <name>: Remove a resource\n");
		printf("help: Ask for help (you may have already found this one lol)\n");
		printf("exit: Exit aed\n");
	}
	else if(strcmp(help_section, "args") == 0){
		printf("Please use the following format:\n");
		printf("./aed [-d <working_directory>]\n");
	}
}

inline void lsAsset(const char* dir){
	File* file_cursor = ReadDirectory(dir);
	while(file_cursor != nullptr){
		int dir_name_len = strlen(file_cursor->name);
		const int file_extension_num = 5;
		char file_extensions[file_extension_num][5] = {".fnt", ".mus", ".snd", ".pxl", ".cut"};

		for(int i = 0; i < file_extension_num; i++){
			int file_extension_len = strlen(file_extensions[i]);
			if(strcmp(file_cursor->name, ".") != 0 && strcmp(file_cursor->name, "..") != 0){
				if(file_cursor->type == FILE_TYPE::DIRECTORY){
					printf("Directory found: %s", file_cursor->name);

					//Recurse into the new dir to look for more resource files
					int new_dir_name_len = dir_name_len + strlen(dir) + 2;
					char new_dir_name[new_dir_name_len];
					strcpy(new_dir_name, dir);
					strcat(new_dir_name, "/");
					strcat(new_dir_name, file_cursor->name);
					new_dir_name[new_dir_name_len - 1] = '\0';
					lsAsset(new_dir_name);
				}
				else if(file_cursor->type == FILE_TYPE::REG_FILE
				    && (dir_name_len >= file_extension_len && strncmp(file_cursor->name + (dir_name_len - file_extension_len), file_extensions[i], file_extension_len))){
					printf("File found: %s", file_cursor->name);
				}
			}
		}

		File* tmp = file_cursor;
		file_cursor = file_cursor->next;
		free(tmp);
	}
}

char* loadAsset(FILE* file, uint8_t* resource_type){
	fread(&resource_type, sizeof(*resource_type), 1, file);
	if(*resource_type == RESOURCE_TYPE::BMP){
		return loadBMP(file);
	}
	else if(*resource_type == RESOURCE_TYPE::FONT){
		return loadFont(file);
	}
	else if(*resource_type == RESOURCE_TYPE::SOUND){
		return loadSound(file);
	}
	else if(*resource_type == RESOURCE_TYPE::MUSIC){
		return loadMusic(file);
	}
}

char* editAsset(char* name, uint8_t resource_type){

}

char* addAsset(char* name, uint8_t resource_type){
	//Allocate a string with extra space for the file extension
	int new_file_len = strlen(name) + 4;
	char new_file[new_file_len + 1];
	memset(new_file, 0, new_file_len + 1);
	memcpy(new_file, name, strlen(name));

	if(resource_type == RESOURCE_TYPE::BMP){
		strcat(new_file, ".spr");
		
		printf("Please enter the surface's file name:\n >");
		char surface_file[MAX_LINE_LEN];
		fgets(surface_file, MAX_LINE_LEN, stdin);
		
		//Build the surface
		SDL_Surface* new_surface = IMG_Load(surface_file);

		FILE* file = fopen(new_file, "wb");

		//Identifier len
		uint16_t identifier_len = strlen(name);
		uint16_t identifier_len_swapped = EndianSwap(&identifier_len);

		//Identifier
		fwrite(&identifier_len_swapped, 2, 1, file);
		fwrite(name, 1, identifier_len, file);
		
		SerializeSurface(file, new_surface);
		fclose(file);
	}
	else if(resource_type == RESOURCE_TYPE::FONT){
		strcat(new_file, ".fnt");
	}
	else if(resource_type == RESOURCE_TYPE::SOUND){
		strcat(new_file, ".snd");

		printf("Please enter the sound's file name:\n >");
		char sound_file[MAX_LINE_LEN];
		fgets(sound_file, MAX_LINE_LEN, stdin);
		
		//Build the surface
		Mix_Chunk* chunk = Mix_LoadWAV(sound_file);
		Sound sound;
		sound.name = name;
		sound.sample = chunk;

		FILE* file = fopen(new_file, "wb");

		SerializeSound(file, &sound);
		fclose(file);
	}
	else if(resource_type == RESOURCE_TYPE::MUSIC){
		strcat(new_file, ".mus");
	}
}

int main(int argc, char** argv){
	char* working_dir = ".";
	for(int i = 1; i < argc; i++){
		if(strcmp(argv[i], "-d") == 0){
			if(i + 1 < argc){
				working_dir = argv[i++];
			}
		}
		else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0){
			printHelp("args");
			return 0;
		}
		else{
			printHelp("args");
			return -1;
		}
	}

	char command_str[MAX_LINE_LEN];
	do{
		printf("> ");
		fgets(command_str, MAX_LINE_LEN, stdin);
		char** command_args = getArgs(command_str, " ");
		if(command_args == nullptr || command_args[0] == nullptr){
			printHelp("main_loop");
			continue;
		}

		ToLower(command_args[0]);
		if(strcmp(command_args[0], "help") == 0){
			printHelp("main_loop");
		}
		else if(strcmp(command_args[0], "ls") == 0){
			if(command_args[1] != nullptr){
				lsAsset(command_args[1]);
			}
			else{
				lsAsset(working_dir);
			}
		}
		else if(command_args[2] != nullptr && strcmp(command_args[0], "add") == 0){
			ToLower(command_args[1]);
			if(strcmp(command_args[1], "sprite") == 0){
				addAsset(command_args[2], RESOURCE_TYPE::BMP);
			}
			else if(strcmp(command_args[1], "font") == 0){
				addAsset(command_args[2], RESOURCE_TYPE::FONT);
			}
			else if(strcmp(command_args[1], "sound") == 0){
				addAsset(command_args[2], RESOURCE_TYPE::SOUND);
			}
			else if(strcmp(command_args[1], "music") == 0){
				addAsset(command_args[2], RESOURCE_TYPE::MUSIC);
			}
		}
		else if(command_args[1] != nullptr && strcmp(command_args[0], "edit") == 0){
			FILE* file = fopen(command_args[1], "rb");
			if(file != nullptr){
				uint8_t resource_type;
				char* key = loadAsset(file, &resource_type);
				addAsset(command_args[2], resource_type);
			}
			else{
				printf("%s could not be opened for reading; double-check the path!\n", command_args[1]);
			}
		}
		else if(command_args[1] != nullptr && strcmp(command_args[0], "rm")){
			for(int i = 1; command_args[i] != nullptr; i++){
				if(remove(command_args[i]) == 0){
					printf("%s removed successfully!\n", command_args[i]);
				}
				else{
					printf("Could not remove %s; errored out with message: %s\n", command_args[i], strerror(errno));
				}
			}
		}
		else{
			printHelp("main_loop");
		}

		for(int i = 0; command_args[i] != nullptr; i++){
			free(command_args);
		}

	} while(strcmp(command_str, "quit") == 0);

	printf("Enjoy using the new resources; come back soon!\n");

	return 0;
}