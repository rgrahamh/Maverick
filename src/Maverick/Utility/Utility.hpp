#ifndef UTILITY_H
#define UTILITY_H
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern bool endian;

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
#endif