#include <inttypes.h>

inline uint64_t EndianSwap(uint64_t* input, bool endianness){
	if(endianness == 0){
		return *input;
	}
	else{
		uint64_t output;
		for(int i = 0; i < 8; i++){
			(&output)[i] = input[8 - i];
		}
		return output;
	}
}

inline int64_t EndianSwap(int64_t* input, bool endianness){
	if(endianness == 0){
		return *input;
	}
	else{
		int64_t output;
		for(int i = 0; i < 8; i++){
			(&output)[i] = input[8 - i];
		}
		return output;
	}
}

inline uint32_t EndianSwap(uint32_t* input, bool endianness){
	if(endianness == 0){
		return *input;
	}
	else{
		uint32_t output;
		for(int i = 0; i < 4; i++){
			(&output)[i] = input[4 - i];
		}
		return output;
	}
}

inline int32_t EndianSwap(int32_t* input, bool endianness){
	if(endianness == 0){
		return *input;
	}
	else{
		int32_t output;
		for(int i = 0; i < 4; i++){
			(&output)[i] = input[4 - i];
		}
		return output;
	}
}

inline uint16_t EndianSwap(uint16_t* input, bool endianness){
	if(endianness == 0){
		return *input;
	}
	else{
		uint16_t output;
		(&output)[0] = input[1];
		(&output)[1] = input[0];
		return output;
	}
}

inline int16_t EndianSwap(int16_t* input, bool endianness){
	if(endianness == 0){
		return *input;
	}
	else{
		int16_t output;
		(&output)[0] = input[1];
		(&output)[1] = input[0];
		return output;
	}
}