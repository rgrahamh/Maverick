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

#define likely(x) __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)
static const double ZOOM_RATE = 0.08;
static const double ROLLBACK_STEP = 6.0;

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
static inline void DrawSDL_Rect(SDL_Renderer* renderer, SDL_Rect& rect){
	SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x + rect.w, rect.y);
	SDL_RenderDrawLine(renderer, rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h);
	SDL_RenderDrawLine(renderer, rect.x, rect.y, rect.x, rect.y + rect.h);
	SDL_RenderDrawLine(renderer, rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h);
}

static inline void DrawSDL_Ellipse(SDL_Renderer* renderer, float center_x, float center_y, float x_radius, float y_radius){
	const float radian_convert = M_PI / 180;
	double degree_iter = 0;
	for(int i = 0; i < 720; i++){
		SDL_RenderDrawPoint(renderer, (x_radius * cos(degree_iter * radian_convert)) + center_x, (y_radius * sin(degree_iter * radian_convert)) + center_y);
		degree_iter += 0.5;
	}
}
#endif