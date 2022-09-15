#ifndef GLOBAL_H
#define GLOBAL_H
#include <inttypes.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <unordered_set>

#include <SDL2/SDL.h>

#if _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif

//Approx. pi; all I can remember rn lol
#define PI 3.1415926535897932384626433832795028841971

#define likely(x) __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)
#define MS_PER_FRAME 167
#define ZOOM_RATE 0.08
#define ROLLBACK_STEP 6.0

//Called in the engine's constructor to set endianness
extern bool endian;

enum FILE_TYPE{
	REG_FILE,
	DIRECTORY,
	SYMLINK,
	OTHER_FILE
};

struct File{
	char* name;
	enum FILE_TYPE type;
	struct File* next;
};

enum RESOURCE_TYPE{
	BMP = 0,
	SOUND,
	MUSIC,
	FONT,
	CUT
};

struct SerializeSet{
	std::unordered_set<std::string> sprite_set;
	std::unordered_set<std::string> audio_set;
	std::unordered_set<std::string> music_set;
	std::unordered_set<std::string> font_set;
};

//The char* hashing definition for std::unordered_maps
struct CharHash{
	unsigned int operator()(const char* key) const{
		unsigned int hash = 0;
		for(int i = 0; key[i] != '\0'; i++){
			hash += key[i];
		}
		return hash;
	}
};

/** Determines endian of the machine running the engine
 * @return 1 if big endian, 0 if little endian
 */
static inline bool getEndian(){
	uint16_t endian_check = 1;
	if(((char*)&endian_check)[0] == 1){
		return 1;
	}
	else{
		return 0;
	}
}

static inline uint64_t EndianSwap(uint64_t input){
	if(endian == 0){
		return input;
	}
	else{
		uint64_t output = 0;
		for(int i = 0; i < 8; i++){
			((uint8_t*)&output)[i] = ((uint8_t*)&input)[8 - i - 1];
		}
		return output;
	}
}

static inline int64_t EndianSwap(int64_t input){
	if(endian == 0){
		return input;
	}
	else{
		int64_t output = 0;
		for(int i = 0; i < 8; i++){
			((uint8_t*)&output)[i] = ((uint8_t*)&input)[8 - i - 1];
		}
		return output;
	}
}

static inline uint32_t EndianSwap(uint32_t input){
	if(endian == 0){
		return input;
	}
	else{
		uint32_t output = 0;
		for(int i = 0; i < 4; i++){
			((uint8_t*)&output)[i] = ((uint8_t*)&input)[4 - i - 1];
		}
		return output;
	}
}

static inline int32_t EndianSwap(int32_t input){
	if(endian == 0){
		return input;
	}
	else{
		int32_t output = 0;
		for(int i = 0; i < 4; i++){
			((uint8_t*)&output)[i] = ((uint8_t*)&input)[4 - i - 1];
		}
		return output;
	}
}

static inline uint16_t EndianSwap(uint16_t input){
	if(endian == 0){
		return input;
	}
	else{
		uint16_t output = 0;
		((uint8_t*)&output)[0] = ((uint8_t*)&input)[1];
		((uint8_t*)&output)[1] = ((uint8_t*)&input)[0];
		return output;
	}
}

static inline int16_t EndianSwap(int16_t input){
	if(endian == 0){
		return input;
	}
	else{
		int16_t output = 0;
		((uint8_t*)&output)[0] = ((uint8_t*)&input)[1];
		((uint8_t*)&output)[1] = ((uint8_t*)&input)[0];
		return output;
	}
}

#define ReadVar(var_name, file_name)\
	fread(&var_name, 1, sizeof(var_name), (file_name));\
	if(sizeof(var_name) > 1){\
		(var_name) = EndianSwap(var_name);\
	}

#define WriteVar(var_name, type, file_name)\
    {\
		type write_swapped_var = var_name;\
		if(sizeof(var_name) > 1){\
			write_swapped_var = EndianSwap(var_name);\
		}\
		fwrite(&write_swapped_var, sizeof(type), 1, (file_name));\
	}

/** Does a deep copy of a string
 * @param str The string to deep copy
 * @return A new string with the contents of a passed-in string
 */
static inline char* StrDeepCopy(const char* str){
    int str_len = strlen(str);
    char* new_str = (char*)malloc(str_len + 1);
    memcpy(new_str, str, str_len);
    new_str[str_len] = '\0';
	return new_str;
}

/** Converts a passed-in string to all lowercase
 * @param str The string to convert to lowercase
 */
static inline void ToLower(char* str){
	for(int i = 0; str[i] != '\0'; i++){
		if(str[i] >= 'A' && str[i] <= 'Z'){
			str[i] += 0x20;
		}
	}
}

/** Converts a passed-in string to all uppercase
 * @param str The string to convert to uppercase
 */
static inline void ToUpper(char* str){
	for(int i = 0; str[i] != '\0'; i++){
		if(str[i] >= 'a' && str[i] <= 'z'){
			str[i] -= 0x20;
		}
	}
}

/** Modifies the passed-in str to add null bytes where delimiters are, and passes back a char*[args] with references to the start of each arg
 * @param str The string to parse
 * @param delims All delims that should be considered params
 * @return A list of references to places in the string where the arguments start
 */
static inline char** GetArgs(char* str, char* delims){
	//The new argument list
	unsigned int max_args = strlen(str) / 2;
	char** arg_lst = (char**)calloc(sizeof(char*), max_args);
	arg_lst[0] = str;
	unsigned int arg_iter = 1;

	//Calculated length pre-loop
	unsigned int len = strlen(str);
	//Tracks the delimiter number
	unsigned int delim_num = strlen(delims);
	//Use
	char capped_quote = '\0';

	//Parsing through the arguments
	for(unsigned int i = 0; i < len; i++){
		//Quote capturing
		if(str[i] == '\"' || str[i] == '\''){
			if(capped_quote == '\0'){
				capped_quote = str[i];
				continue;
			}
			else if(capped_quote == str[i]){
				capped_quote = '\0';
				continue;
			}
		}
		//Dealing with escaped characters
		else if(str[i] == '\\'){
			char* curr_addr = str + i;
			strcpy(curr_addr, curr_addr+1);
			continue;
		}
		//Checking against all specified separation delimeters
		for(unsigned int j = 0; j < delim_num; j++){
			if(str[i] == delims[j] && capped_quote == '\0'){
				str[i] = '\0';

				//If there's a next argument, add it
				if(i + 1 < len){
					arg_lst[arg_iter++] = str + i + 1;
				}

				//If we go over the max iterations
				if(arg_iter > max_args){
					return NULL;
				}
			}
		}
	}
	if(capped_quote == '\"' || capped_quote == '\''){
		return NULL;
	}

	for(unsigned int i = 0; i < arg_iter; i++){
		int arg_len = strlen(arg_lst[i]);
		if(arg_len >= 2 && ((arg_lst[i][0] == '\'' && arg_lst[i][arg_len-1] == '\'') || (arg_lst[i][0] == '\"' && arg_lst[i][arg_len-1] == '\"'))){
			arg_lst[i][arg_len-1] = '\0';
			arg_lst[i] = arg_lst[i] + 1;
		}
	}

	return arg_lst;
}

static inline void Normalize2DVector(double* x_force, double* y_force){
	double hypotenuse = sqrt(pow(*x_force, 2) + pow(*y_force, 2));
	*x_force /= hypotenuse;
	*y_force /= hypotenuse;
}

static inline void Normalize3DVector(double* x_force, double* y_force, double* z_force){
	double hypotenuse = sqrt(pow(*x_force, 2) + pow(*y_force, 2) + pow(*z_force, 2));

	*x_force /= hypotenuse;
	*y_force /= hypotenuse;
	*z_force /= hypotenuse;
}

static inline int SerializeSurface(FILE* file, SDL_Surface* surface){
	if(file == nullptr || surface == nullptr){
		return -1;
	}

	//Width/Height are naturally ints (so size varies), meaning we may need to truncate first
	uint32_t width = surface->w;
	uint32_t height = surface->h;

	//Bit depth of the image
	uint8_t depth = surface->format->BitsPerPixel;

	//The RGBA masks
	uint32_t rmask = surface->format->Rmask;
	uint32_t gmask = surface->format->Gmask;
	uint32_t bmask = surface->format->Bmask;
	uint32_t amask = surface->format->Amask;

	//Write the image header info
	WriteVar(width, uint32_t, file);
	WriteVar(height, uint32_t, file);
	WriteVar(depth, uint8_t, file);
	WriteVar(rmask, uint32_t, file)
	WriteVar(gmask, uint32_t, file)
	WriteVar(bmask, uint32_t, file)
	WriteVar(amask, uint32_t, file)

	//Write the actual image data ((w * h * bpp) bytes)
	fwrite(&surface->format->BytesPerPixel, 1, sizeof(surface->format->BytesPerPixel), file);
	fwrite(surface->pixels, 1, width * height * surface->format->BytesPerPixel, file);
	
	return 0;
}

static inline File* ReadDirectory(const char* dir_name){
//If Windows
#if _WIN32
	//Figure this out when I compile on a Windows machine ig lol
	return nullptr;
//else if __linux__ or __APPLE__ (assume some POSIX standard machine)
#else
	File* files = nullptr;
	DIR* dir = opendir(dir_name);
	struct dirent* dir_cursor = readdir(dir);
	while(dir_cursor != nullptr){
		if(files == nullptr){
			files = new File;
			files->next = nullptr;
		}
		else{
			File* new_file = new File;
			new_file->next = files;
			files = new_file;
		}

		files->name = StrDeepCopy(dir_cursor->d_name);
		switch(dir_cursor->d_type){
			case(DT_REG):{
				files->type = FILE_TYPE::REG_FILE;
				break;
			}
			case(DT_DIR):{
				files->type = FILE_TYPE::DIRECTORY;
				break;
			}
			case(DT_LNK):{
				files->type = FILE_TYPE::SYMLINK;
				break;
			}
			default:{
				files->type = FILE_TYPE::REG_FILE;
			}
		}
		dir_cursor = readdir(dir);
	}

	closedir(dir);

	return files;
	
#endif
}

static inline void DrawSDL_Rect(SDL_Renderer* renderer, SDL_Rect& rect){
	SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x + rect.w, rect.y);
	SDL_RenderDrawLine(renderer, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h);
	SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x, rect.y + rect.h);
	SDL_RenderDrawLine(renderer, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h);
}

static inline void DrawSDL_Ellipse(SDL_Renderer* renderer, float center_x, float center_y, float x_radius, float y_radius){
	const float radian_convert = PI / 180;
	double degree_iter = 0;
	for(int i = 0; i < 720; i++){
		SDL_RenderDrawPoint(renderer, (x_radius * cos(degree_iter * radian_convert)) + center_x, (y_radius * sin(degree_iter * radian_convert)) + center_y);
		degree_iter += 0.5;
	}
}
#endif