#ifndef GLOBAL_H
#define GLOBAL_H
#include <inttypes.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <unordered_set>

#include <SDL2/SDL.h>

extern bool endian;

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

static inline uint64_t EndianSwap(uint64_t* input){
	if(endian == 0){
		return *input;
	}
	else{
		uint64_t output;
		for(int i = 0; i < 8; i++){
			((uint8_t*)&output)[i] = ((uint8_t*)input)[8 - i - 1];
		}
		return output;
	}
}

static inline int64_t EndianSwap(int64_t* input){
	if(endian == 0){
		return *input;
	}
	else{
		int64_t output;
		for(int i = 0; i < 8; i++){
			((uint8_t*)&output)[i] = ((uint8_t*)input)[8 - i - 1];
		}
		return output;
	}
}

static inline uint32_t EndianSwap(uint32_t* input){
	if(endian == 0){
		return *input;
	}
	else{
		uint32_t output;
		for(int i = 0; i < 4; i++){
			((uint8_t*)&output)[i] = ((uint8_t*)input)[4 - i - 1];
		}
		return output;
	}
}

static inline int32_t EndianSwap(int32_t* input){
	if(endian == 0){
		return *input;
	}
	else{
		int32_t output;
		for(int i = 0; i < 4; i++){
			((uint8_t*)&output)[i] = ((uint8_t*)input)[4 - i - 1];
		}
		return output;
	}
}

static inline uint16_t EndianSwap(uint16_t* input){
	if(endian == 0){
		return *input;
	}
	else{
		uint16_t output;
		((uint8_t*)&output)[0] = ((uint8_t*)input)[1];
		((uint8_t*)&output)[1] = ((uint8_t*)input)[0];
		return output;
	}
}

static inline int16_t EndianSwap(int16_t* input){
	if(endian == 0){
		return *input;
	}
	else{
		int16_t output;
		((uint8_t*)&output)[0] = ((uint8_t*)input)[1];
		((uint8_t*)&output)[1] = ((uint8_t*)input)[0];
		return output;
	}
}

static inline char* StrDeepCopy(const char* str){
    int str_len = strlen(str);
    char* new_str = (char*)malloc(str_len + 1);
    memcpy(new_str, str, str_len);
    new_str[str_len] = '\0';
	return new_str;
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
	uint32_t width_swap = EndianSwap(&width);
	uint32_t height = surface->h;
	uint32_t height_swap = EndianSwap(&height);

	//Bit depth of the image
	uint8_t depth = surface->format->BitsPerPixel;

	//The RGBA masks
	uint32_t rmask = EndianSwap(&surface->format->Rmask);
	uint32_t gmask = EndianSwap(&surface->format->Gmask);
	uint32_t bmask = EndianSwap(&surface->format->Bmask);
	uint32_t amask = EndianSwap(&surface->format->Amask);

	uint8_t asset_type = RESOURCE_TYPE::BMP;
	fwrite(&asset_type, 1, 1, file);

	//Write the image header info
	fwrite(&width_swap, sizeof(width_swap), 1, file);
	fwrite(&height_swap, sizeof(height_swap), 1, file);
	fwrite(&depth, 1, 1, file);
	fwrite(&rmask, sizeof(rmask), 1, file);
	fwrite(&gmask, sizeof(gmask), 1, file);
	fwrite(&bmask, sizeof(bmask), 1, file);
	fwrite(&amask, sizeof(amask), 1, file);

	//Write the actual image data ((w * h * bpp) bytes)
	fwrite(&surface->format->BytesPerPixel, 1, sizeof(surface->format->BytesPerPixel), file);
	fwrite(surface->pixels, 1, width * height * surface->format->BytesPerPixel, file);
	 return 0;
}
#endif