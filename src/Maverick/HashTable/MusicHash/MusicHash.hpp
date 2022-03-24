#ifndef MUSICHASH_HPP
#define MUSICHASH_HPP

#include <string.h>
#include <SDL2/SDL.h>
#include <atomic>

#include "../../Audio/Music/Music.hpp"
#include "../HashTable.hpp"

typedef struct MusicHashEntry{
	char* key;
	Music* music;
	struct MusicHashEntry* next;
} MHEntry;

class MusicHash : public HashTable{
	public:
		MusicHash(unsigned int size);
		~MusicHash();

		void add(const char* key, Music* surface);
		bool has(const char* key);
		Music* get(const char* key);
	
	private:
		std::atomic<MHEntry**> table;
};
#endif