#define SDL_MAIN_HANDLED
#include "./red.hpp"
#include "../Maverick/FileHandler/Loader/Loader.hpp"
#include "../Maverick/Engine/Engine.hpp"

extern Engine* engine;

/** Prints out help for the program
 * @param help_section The section of the app you'd like help for
 */
inline void printHelp(char* help_section){
	if(strcmp(help_section, "main_loop") == 0){
		printf("You may use the following options:\n");
		printf("ls: Show resource files in the input directory\n");
		printf("add <resource_type> <name>: Add a resource\n");
		printf("edit <file_name>: Edit a resource\n");
		printf("rm <file_name>: Remove a resource\n");
		printf("help: Ask for help (you may have already found this one lol)\n");
		printf("quit: Quit red\n\n");
		printf("Resouce types:\n");
		printf("sprite\n");
		printf("font\n");
		printf("sound\n");
		printf("music\n");
	}
	else if(strcmp(help_section, "args") == 0){
		printf("Please use the following format:\n");
		printf("./aed [-d <working_directory>]\n");
	}
	else if(strcmp(help_section, "font") == 0){
		printf("You may use the following options:\n");
		printf("edit <style>: Edit a style for the font. Valid styles are standard, italic, or bold\n");
		printf("save: Save changes and exit\n");
		printf("quit: Abort changes and exit\n");
	}
	else if(strcmp(help_section, "font_style") == 0){
		printf("You may use the following options:\n");
		printf("add <char> <file_name>: Add a new character-surface mapping for the font\n");
		printf("rm <char>: Remove the surface for a character\n");
		printf("import <file_pattern>: Try a bulk import with the standard file structure (EXPERIMENTAL)\n");
		printf("save: Save changes and exit\n");
		printf("quit: Abort changes and exit\n");
	}
	else if(strcmp(help_section, "music") == 0){
		printf("You may use the following options:\n");
		printf("add <instrument_name> <file_name>: Add an instrument to the song, where <file_name> is a .wav\n");
		printf("save: Save changes and exit\n");
		printf("quit: Abort changes and exit\n");
	}
}

/** Prompt the user for input
 * @param prompt The text prompt you'd like to display
 * @param app_location The text that will display in the box before the prompt arrow
 * @param response A char* that will contain the response after calling this function
 */
inline void promptUser(char* prompt, char* app_location, char* response){
	printf("\n%s\n[%s]> ", prompt, app_location);
	fgets(response, MAX_LINE_LEN, stdin);
	char* first_newline = strchr(response, '\n');
	if(first_newline != nullptr){
		*first_newline = '\0';
	}
}

/** Does a recursive ls for resource files (useful for finding pre-existing resources)
 * @param dir The directory you'd like to start the ls in
 */
inline void lsAsset(const char* dir){
	File* file_cursor = ReadDirectory(dir);
	while(file_cursor != nullptr){
		int dir_name_len = strlen(file_cursor->name);
		const int file_extension_num = 5;
		char file_extensions[file_extension_num][5] = {".fnt", ".mus", ".snd", ".spr", ".cut"};

		if(strcmp(file_cursor->name, ".") != 0 && strcmp(file_cursor->name, "..") != 0){
			if(file_cursor->type == FILE_TYPE::DIRECTORY){
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

/** Loads an asset into the engine
 * @param file The file you're reading the asset from
 * @param resource_type_ptr A ptr to the resource type of the asset (will be populated with the asset type)
 * @return The asset if it exists in the engine, otherwise a nullptr
 */
void* loadAsset(FILE* file, uint8_t* resource_type_ptr, char* key_buff, unsigned int max_len){
	fread(resource_type_ptr, 1, 1, file);
	if(*resource_type_ptr == RESOURCE_TYPE::BMP){
		return loadBMP(file);
	}
	else if(*resource_type_ptr == RESOURCE_TYPE::FONT){
		return loadFont(file);
	}
	else if(*resource_type_ptr == RESOURCE_TYPE::SOUND){
		return loadSound(file);
	}
	else if(*resource_type_ptr == RESOURCE_TYPE::MUSIC){
		return loadMusic(file);
	}
	return nullptr;
}

/** Gets an asset from the engine
 * @param key The asset key
 * @param resource_type The asset type
 * @return The asset if it exists in the engine, otherwise a nullptr
 */
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

/** Tries adding a new character based upon a file path & information regarding what the characer is
 * @param font A pointer to the font object
 * @param style The style of the font
 * @param file_path The file path of the surface
 * @param val The char val you'd like to set
 * @return -1 on error, 0 otherwise
 */
inline int addFontCharacter(Font* font, FONT_STYLE style, const char* file_path, char val){
	SDL_Surface* new_surface = IMG_Load(file_path);
	if(new_surface == nullptr){
		return -1;
	}

	font->setCharacter(val, new_surface, style);
	return 0;
}

/** Tries a bulk import based upon a file pattern with a range of characters
 * @param start The starting character
 * @param end The ending character (inclusive)
 * @param file_pattern The file pattern leading up to the character
 * @param font A pointer to the font object
 * @param style The style of the font
 * @return The number of imported characters
 */
inline int importChars(char start, char end, const char* file_pattern, Font* font, FONT_STYLE style){
	if(start > end){
		return 0;
	}

	int import_num = 0;
	int file_pattern_offset = strlen(file_pattern);
	//Checking against null byte is needed here, since we'll never print a null byte & 255 might be used in the extended char set
	for(char i = start; i <= end && i != '\0'; i++){
		char file_name[MAX_LINE_LEN + 7];
		file_name[0] = '\0';
		strcpy(file_name, file_pattern);
		sprintf(file_name + file_pattern_offset, "_%c.bmp", i);

		if(addFontCharacter(font, style, file_name, i) == 0){
			printf("Added %c from %s\n", i, file_name);
			import_num++;
		}
	}

	return import_num;
}

/** Edits a font style
 * @param font The font you wish to edit
 * @param font_style The font style enum
 * @return If you want to save the font
 */
inline void editFontStyle(Font* font, FONT_STYLE font_style){
	char command_str[MAX_LINE_LEN];
	char** command_args = nullptr;
	do{
		promptUser("Enter in a character and the associated filename (.bmp), or use a file pattern to import in bulk", "font-style", command_str);
		command_args = GetArgs(command_str, " ");
		ToLower(command_args[0]);

		//Doing an edit operation for a given font style
		if(strcmp(command_args[0], "add") == 0 && command_args[2] != nullptr){
			if(addFontCharacter(font, font_style, command_args[2], command_args[1][0]) == 0){
				printf("Added %c from %s\n", command_args[1][0], command_args[2]);
			}
			else{
				printf("Cannot open file %s; please check the path\n", command_args[2]);
			}
		}
		else if(strcmp(command_args[0], "rm") == 0 && command_args[1] != nullptr){
			font->removeCharacter(command_args[1][0], font_style);
		}
		else if(strcmp(command_args[0], "import") == 0 && command_args[1] != nullptr){
			char file_pattern[MAX_LINE_LEN];
			file_pattern[0] = '\0';
			strcpy(file_pattern, command_args[1]);
			int file_pattern_offset = strlen(file_pattern);

			//Try an import on everything in the printable character set
			importChars(' ', '~', file_pattern, font, font_style);

			char caps_file_pattern[MAX_LINE_LEN + 3];
			caps_file_pattern[0] = '\0';
			strcpy(caps_file_pattern, file_pattern);
			strcat(caps_file_pattern, "CAP");
			//Try an import on the caps letters exceptions
			importChars('A', 'Z', caps_file_pattern, font, font_style);

			//Try the exception files
			const unsigned int exception_num = 8;
			char* except_str[] = {"_backslash", "_colon", "_comma", "_forwardslash", "_greaterthan", "_lessthan", "_openquote", "_questionmark", "_star"};
			char except_char[] = {'\\', ':', ',', '/', '>', '<', '"', '?', '*'};
			for(unsigned int i = 0; i < exception_num; i++){
				char file_path[MAX_LINE_LEN + 15];
				file_path[0] = '\0';
				strcpy(file_path, file_pattern);
				sprintf(file_path + file_pattern_offset, "_%s.bmp", except_str[i]);

				if(addFontCharacter(font, font_style, file_path, except_char[i]) == 0){
					printf("Added %c from %s\n", except_char[i], file_path);
				}
			}
		}
		else if(strcmp(command_args[0], "help") == 0){
			printHelp("font_style");
		}
		else if(strcmp(command_args[0], "save") != 0){
			printf("Invalid input detected; please try again\n");
		}
	
		free(command_args);
	} while(strcmp(command_str, "save") != 0);
}

/** Edits a font
 * @param font The font you wish to edit
 * @return If you want to save the font
 */
bool editFont(Font* font){
	char command_str[MAX_LINE_LEN];
	char** command_args = nullptr;
	do{
		promptUser("What would you like to edit? You may edit the font group (standard/bold/italic) or spacing, or use save/quit to save or abort.", "font", command_str);
		command_args = GetArgs(command_str, " ");
		ToLower(command_args[0]);

		//Doing an edit operation for a given font style
		if(strcmp(command_args[0], "edit") == 0 && command_args[1] != nullptr){
			uint8_t font_style = FONT_STYLE::STANDARD_STYLE;
			if(strcmp(command_args[1], "standard") == 0){
				font_style = FONT_STYLE::STANDARD_STYLE;
			}
			else if(strcmp(command_args[1], "italic") == 0){
				font_style = FONT_STYLE::ITALIC_STYLE;
			}
			else if(strcmp(command_args[1], "bold") == 0){
				font_style = FONT_STYLE::BOLD_STYLE;
			}
			else if(strcmp(command_args[1], "spacing") == 0){
				char spacing_str[MAX_LINE_LEN];
				promptUser("Please specify the font spacing:", "font", spacing_str);
				font->setSpacing(strtoul(spacing_str, nullptr, 10));
				printf("Set font spacing to %d\n", font->getSpacing());
				continue;
			}

			editFontStyle(font, (FONT_STYLE)font_style);
		}
		else if(strcmp(command_args[0], "help") == 0){
			printHelp("font");
		}
		else if(strcmp(command_args[0], "quit") != 0 && strcmp(command_args[0], "save") != 0){
			printf("Invalid input detected; please try again\n");
		}

		free(command_args);
	} while(strcmp(command_str, "quit") != 0 && strcmp(command_str, "save") != 0);

	return (strcmp(command_str, "save") == 0);
}

/** Edits music
 * @param music The music you wish to edit
 * @return If you want to save the music
 */
bool editMusic(Music* music){
	char command_str[MAX_LINE_LEN];
	char** command_args = nullptr;
	do{
		promptUser("What music group would you like to edit (standard/bold/italic)? Or use save to save changes, or quit to abort.", "music", command_str);
		command_args = GetArgs(command_str, " ");
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
		else if(strcmp(command_args[0], "help") == 0){
			printHelp("music");
		}
		else if(strcmp(command_args[0], "quit") != 0 && strcmp(command_args[0], "save") != 0){
			printf("Invalid input detected; please try again\n");
		}
	
		free(command_args);
	} while(strcmp(command_str, "quit") != 0 && strcmp(command_str, "save") != 0);

	return (strcmp(command_str, "save") == 0);
}

/** Edits an asset
 * @param file_name The name of the file you'd like to save to
 * @param resource_type The type of resource you're editing
 * @param base_resource The base resource you're editing
 */
void editAsset(char* file_name, uint8_t resource_type, void* base_resource){
	if(resource_type == RESOURCE_TYPE::BMP){
		printf("Sprites shouldn't be edited; if you want to change a sound, do a rm & add\n");
	}
	else if(resource_type == RESOURCE_TYPE::FONT){
		if(editFont((Font*)base_resource)){
			FILE* file = fopen(file_name, "wb");
			((Font*)base_resource)->serialize(file);
			fclose(file);
		}
	}
	else if(resource_type == RESOURCE_TYPE::SOUND){
		printf("Sounds shouldn't be edited; if you want to change a sound, do a rm & add\n");
	}
	else if(resource_type == RESOURCE_TYPE::MUSIC){
		if(editMusic((Music*)base_resource)){
			FILE* file = fopen(file_name, "wb");
			((Music*)base_resource)->serialize(file);
			fclose(file);
		}
	}
}

/** Adds an asset
 * @param name The name of the asset you'd like to add
 * @param resource_type The type of resource you're editing
 */
void addAsset(char* name, uint8_t resource_type){
	//Allocate a string with extra space for the file extension
	int new_file_len = strlen(name) + 4;
	char new_file[new_file_len + 1];
	memset(new_file, 0, new_file_len + 1);
	memcpy(new_file, name, strlen(name));

	if(resource_type == RESOURCE_TYPE::BMP){
		strcat(new_file, ".spr");
		
		char sprite_file[MAX_LINE_LEN];
		promptUser("Please enter the sprite's file name:", "sprite", sprite_file);
		
		//Build the sprite
		SDL_Surface* new_sprite = IMG_Load(sprite_file);
		if(new_sprite == nullptr){
			printf("Couldn't open the sprite at %s; will not write to fiile\n", sprite_file);
		}

		FILE* file = fopen(new_file, "wb");

		fwrite(&resource_type, sizeof(resource_type), 1, file);

		//Identifier
		uint16_t identifier_len = strlen(name);
		WriteVar(identifier_len, uint16_t, file);
		fwrite(name, 1, identifier_len, file);
		
		if(SerializeSurface(file, new_sprite) != 0){
			printf("Something went wrong in serialization; could not write the sprite!\n");
		}
		else{
			printf("Surface written successfully!\n");
		}
		fclose(file);
	}
	else if(resource_type == RESOURCE_TYPE::FONT){
		Font font(name);
		strcat(new_file, ".fnt");
		editAsset(new_file, resource_type, &font);
	}
	else if(resource_type == RESOURCE_TYPE::SOUND){
		strcat(new_file, ".snd");

		char sound_file[MAX_LINE_LEN];
		promptUser("Please enter the sound's file name:", "sound", sound_file);
		
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

		fwrite(&resource_type, sizeof(resource_type), 1, file);

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
	engine = new Engine();
	SDL_DestroyWindow(engine->getWindow());

	printf("Welcome to red, your Resource EDitor!\n");
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
		promptUser("What would you like to do?", "red", command_str);
		char** command_args = GetArgs(command_str, " ");
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
			if(strcmp(command_args[1], "surface") == 0){
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
				char key[MAX_LINE_LEN];
				void* resource = loadAsset(file, &resource_type, key, MAX_LINE_LEN);
				editAsset(command_args[1], resource_type, resource);
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

	printf("\nEnjoy using the new resources; come back soon!\n");

	return 0;
}