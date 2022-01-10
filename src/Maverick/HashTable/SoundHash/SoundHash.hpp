#ifndef SOUNDHASH_HPP
#define SOUNDHASH_HPP

#include <string.h>
#include <SDL2/SDL.h>
#include <atomic>

#include "../../Audio/SoundBoard.hpp"
#include "../HashTable.hpp"

typedef struct SoundHashEntry{
	char* key;
	Sound* sound;
	struct SoundHashEntry* next;
} SHEntry;

class SoundHash : public HashTable{
	public:
		SoundHash(unsigned int size);
		~SoundHash();

		void add(const char* key, Sound* sound);
		bool has(const char* key);
		Sound* get(const char* key);
	
	private:
		std::atomic<SHEntry**> table;
};
#endif