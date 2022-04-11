#include "./red.hpp"
#include "../Maverick/FileHandler/Loader/Loader.hpp"
#include "../Maverick/Engine/Engine.hpp"

extern Engine* engine;

inline int printHelp(char* help_section){
	if(strcmp(help_section, "main_loop") == 0){
		printf("You may use the following options:\n");
		printf("ls: Show resource files in the input directory\n");
		printf("add <resource_type> <name>: Add a resource\n");
		printf("edit <file_name>: Edit a resource\n");
		printf("rm <file_name>: Remove a resource\n");
		printf("help: Ask for help (you may have already found this one lol)\n");
		printf("quit: Quit red\n\n");
	}
	else if(strcmp(help_section, "args") == 0){
		printf("Please use the following format:\n");
		printf("./aed [-d <working_directory>]\n\n");
	}
}

inline void promptUser(char* prompt, char* response){
	printf("\n%s\n> ", prompt);
	fgets(response, MAX_LINE_LEN, stdin);
	char* first_newline = strchr(response, '\n');
	if(first_newline != nullptr){
		*first_newline = '\0';
	}
}

inline void lsAsset(const char* dir){
	File* file_cursor = ReadDirectory(dir);
	while(file_cursor != nullptr){
		int dir_name_len = strlen(file_cursor->name);
		const int file_extension_num = 5;
		char file_extensions[file_extension_num][5] = {".fnt", ".mus", ".snd", ".pxl", ".cut"};

		if(strcmp(file_cursor->name, ".") != 0 && strcmp(file_cursor->name, "..") != 0){
			if(file_cursor->type == FILE_TYPE::DIRECTORY){
				printf("Directory found: %s\n", file_cursor->name);

				//Recurse into the new dir to look for more resource files
				int new_dir_name_len = dir_name_len + strlen(dir) + 2;
				char new_dir_name[new_dir_name_len];
				strcpy(new_dir_name, dir);
				strcat(new_dir_name, "/");
				strcat(new_dir_name, file_cursor->name);
				new_dir_name[new_dir_name_len - 1] = '\0';
				lsAsset(new_dir_name);
			}
			else{
				for(int i = 0; i < file_extension_num; i++){
					int file_extension_len = strlen(file_extensions[i]);
					if(file_cursor->type == FILE_TYPE::REG_FILE
						&& (dir_name_len >= file_extension_len && strncmp(file_cursor->name + (dir_name_len - file_extension_len), file_extensions[i], file_extension_len) == 0)){
						printf("File found: %s\n", file_cursor->name);
					}
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
	return nullptr;
}

void* getAsset(char* key, uint8_t resource_type){
	if(resource_type == RESOURCE_TYPE::BMP){
		return engine->getSurface(key);
	}
	else if(resource_type == RESOURCE_TYPE::FONT){
		return engine->getFont(key);
	}
	else if(resource_type == RESOURCE_TYPE::SOUND){
		return engine->getSound(key);
	}
	else if(resource_type == RESOURCE_TYPE::MUSIC){
		return engine->getMusic(key);
	}
	return nullptr;
}

/** Edits a font
 * @param font The font you wish to edit
 * @param font_style The font style enum
 * @return If you want to save the font
 */
inline void editFontStyle(Font* font, FONT_STYLE font_style){
	char command_str[MAX_LINE_LEN];
	char** command_args = nullptr;
	do{
		promptUser("Enter in a character, and the associated filename (.bmp)", command_str);
		command_args = getArgs(command_str, " ");
		ToLower(command_args[0]);

		//Doing an edit operation for a given font style
		if(strcmp(command_args[0], "add") == 0 && command_args[2] != nullptr){
			SDL_Surface* new_surface = IMG_Load(command_args[2]);
			if(new_surface != nullptr){
				font->setCharacter(command_args[1][0], new_surface, font_style);
				printf("Set %c to %s\n", command_args[1][0], command_args[2]);
			}
			else{
				printf("Cannot open file %s; please check the path\n >", command_args[2]);
			}
		}
		else if(strcmp(command_args[0], "rm") == 0 && command_args[1] != nullptr){
			font->removeCharacter(command_args[1][0], font_style);
		}
		else if(strcmp(command_args[0], "import") == 0 && command_args[1] != nullptr){
			char file_pattern[MAX_LINE_LEN];
			strcpy(file_pattern, command_args[1]);
			int file_pattern_offset = strlen(file_pattern);
		}
		else if(strcmp(command_args[0], "save") != 0){
			printf("Invalid input detected; please try again");
		}
	
		free(command_args);
	} while(strcmp(command_args[0], "save") != 0);
}

/** Edits a font
 * @param font The font you wish to edit
 * @return If you want to save the font
 */
bool editFont(Font* font){
	char command_str[MAX_LINE_LEN];
	char** command_args = nullptr;
	do{
		promptUser("What font group would you like to edit (standard/bold/italic)? Or use save to save changes, or quit to abort.", command_str);
		command_args = getArgs(command_str, " ");
		ToLower(command_args[0]);

		//Doing an edit operation for a given font style
		if(strcmp(command_args[0], "edit") && command_args[1] != nullptr){
			uint8_t font_style = FONT_STYLE::STANDARD_STYLE;
			if(strcmp(command_args[1], "standard")){
				font_style = FONT_STYLE::STANDARD_STYLE;
			}
			else if(strcmp(command_args[1], "italic")){
				font_style = FONT_STYLE::ITALIC_STYLE;
			}
			else if(strcmp(command_args[1], "bold")){
				font_style = FONT_STYLE::BOLD_STYLE;
			}

			editFontStyle(font, (FONT_STYLE)font_style);
		}
		else if(strcmp(command_args[0], "quit") != 0 && strcmp(command_args[0], "save") != 0){
			printf("Invalid input detected; please try again");
		}
	
		free(command_args);
	} while(strcmp(command_args[0], "quit") != 0 && strcmp(command_args[0], "save") != 0);

	return (strcmp(command_args[0], "save") == 0);
}

/** Edits music
 * @param music The music you wish to edit
 * @return If you want to save the music
 */
bool editMusic(Music* music){
	char command_str[MAX_LINE_LEN];
	char** command_args = nullptr;
	do{
		promptUser("What music group would you like to edit (standard/bold/italic)? Or use save to save changes, or quit to abort.", command_str);
		command_args = getArgs(command_str, " ");
		ToLower(command_args[0]);

		//Doing an edit operation for a given music style
		if(strcmp(command_args[0], "add") && command_args[2] != nullptr){
			Mix_Chunk* chunk = Mix_LoadWAV(command_args[2]);
			if(chunk != nullptr){
				Sound* sound = new Sound;
				sound->name = command_args[1];
				sound->sample = chunk;

				music->addTrack(sound);
			}
			else{
				printf("Couldn't open the sound at %s; will not write to file\n", command_args[2]);
			}
		}
		else if(strcmp(command_args[0], "quit") != 0 && strcmp(command_args[0], "save") != 0){
			printf("Invalid input detected; please try again");
		}
	
		free(command_args);
	} while(strcmp(command_args[0], "quit") != 0 && strcmp(command_args[0], "save") != 0);

	return (strcmp(command_args[0], "save") == 0);
}

void editAsset(char* file_name, uint8_t resource_type, void* base_resource){
if(resource_type == RESOURCE_TYPE::BMP){
	printf("Surfaces shouldn't be edited; if you want to change a sound, do a rm & add");
	}
	else if(resource_type == RESOURCE_TYPE::FONT){
		if(editFont((Font*)base_resource)){
			FILE* file = fopen(file_name, "wb");
			((Font*)base_resource)->serialize(file);
			fclose(file);
		}
	}
	else if(resource_type == RESOURCE_TYPE::SOUND){
		printf("Sounds shouldn't be edited; if you want to change a sound, do a rm & add");
	}
	else if(resource_type == RESOURCE_TYPE::MUSIC){
		if(editMusic((Music*)base_resource)){
			FILE* file = fopen(file_name, "wb");
			((Music*)base_resource)->serialize(file);
			fclose(file);
		}
	}
}

void addAsset(char* name, uint8_t resource_type){
	//Allocate a string with extra space for the file extension
	int new_file_len = strlen(name) + 4;
	char new_file[new_file_len + 1];
	memset(new_file, 0, new_file_len + 1);
	memcpy(new_file, name, strlen(name));

	if(resource_type == RESOURCE_TYPE::BMP){
		strcat(new_file, ".spr");
		
		char surface_file[MAX_LINE_LEN];
		promptUser("Please enter the surface's file name:", surface_file);
		
		//Build the surface
		SDL_Surface* new_surface = IMG_Load(surface_file);
		if(new_surface == nullptr){
			printf("Couldn't open the sound at %s; will not write to fiile\n\n", surface_file);
		}

		FILE* file = fopen(new_file, "wb");

		//Identifier len
		uint16_t identifier_len = strlen(name);
		uint16_t identifier_len_swapped = EndianSwap(&identifier_len);

		//Identifier
		fwrite(&identifier_len_swapped, 2, 1, file);
		fwrite(name, 1, identifier_len, file);
		
		if(SerializeSurface(file, new_surface) != 0){
			printf("Something went wrong in serialization; could not write the surface!\n\n");
		}
		else{
			printf("Surface written successfully!\n\n");
		}
		fclose(file);
	}
	else if(resource_type == RESOURCE_TYPE::FONT){
		strcat(new_file, ".fnt");
		Font font(name);
		editAsset(new_file, resource_type, &font);
	}
	else if(resource_type == RESOURCE_TYPE::SOUND){
		strcat(new_file, ".snd");

		char sound_file[MAX_LINE_LEN];
		promptUser("Please enter the sound's file name:", sound_file);
		
		//Build the surface
		Mix_Chunk* chunk = Mix_LoadWAV(sound_file);
		if(chunk == nullptr){
			printf("Couldn't open the sound at %s; will not write to file\n\n", sound_file);
			return;
		}
		Sound sound;
		sound.name = name;
		sound.sample = chunk;

		FILE* file = fopen(new_file, "wb");

		if(SerializeSound(file, &sound) != 0){
			printf("Something went wrong in serialization; could not write the sound!\n\n");
		}
		else{
			printf("Sound written successfully!\n\n");
		}
		fclose(file);
	}
	else if(resource_type == RESOURCE_TYPE::MUSIC){
		strcat(new_file, ".mus");
		Music music(name);
		editAsset(new_file, resource_type, &music);
	}
}

int main(int argc, char** argv){
	printf("Welcome to red, your REsource Editor!\n");
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
		promptUser("What would you like to do?", command_str);
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
			else{
				printf("Resource type %s not recognized; please use a valid resource\n", command_args[1]);
			}
		}
		else if(command_args[1] != nullptr && strcmp(command_args[0], "edit") == 0){
			FILE* file = fopen(command_args[1], "rb");
			if(file != nullptr){
				uint8_t resource_type;
				char* key = loadAsset(file, &resource_type);
				editAsset(command_args[1], resource_type, getAsset(key, resource_type));
			}
			else{
				printf("%s could not be opened for reading; double-check the path!\n", command_args[1]);
			}
		}
		else if(command_args[1] != nullptr && strcmp(command_args[0], "rm") == 0){
			for(int i = 1; command_args[i] != nullptr; i++){
				if(remove(command_args[i]) == 0){
					printf("%s removed successfully!\n", command_args[i]);
				}
				else{
					printf("Could not remove %s; errored out with message: %s\n", command_args[i], strerror(errno));
				}
			}
		}
		else if(strcmp(command_str, "quit") != 0){
			printHelp("main_loop");
		}

		free(command_args);

	} while(strcmp(command_str, "quit") != 0);

	printf("Enjoy using the new resources; come back soon!\n");

	return 0;
}