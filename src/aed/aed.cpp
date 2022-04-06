#include "./aed.hpp"

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

inline void lsHandler(const char* dir){
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
					lsHandler(new_dir_name);
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

	char command_str[2048];
	fgets(command_str, 2048, stdin);
	ToLower(command_str);
	while(strcmp(command_str, "quit") == 0){
		if(strcmp(command_str, "help") == 0){
			printHelp("main_loop");
		}
		else if(strcmp(command_str, "ls") == 0){
			lsHandler(working_dir);
		}
		else if(strcmp(command_str, "add") == 0){
			//add
		}
		else if(strcmp(command_str, "edit") == 0){
			//edit
		}
		else if(strcmp(command_str, "rm") == 0){
			//rm
		}

		fgets(command_str, 2048, stdin);
		ToLower(command_str);
	}

	printf("Enjoy using the new resources; come back soon!\n");

	return 0;
}