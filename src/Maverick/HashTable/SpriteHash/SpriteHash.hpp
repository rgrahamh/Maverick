#ifndef SPRITEHASH_HPP
#define SPRITEHASH_HPP

#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <atomic>

#include "../HashTable.hpp"

typedef struct SpriteHashEntry{
	char* key;
	SDL_Surface* surface;
	struct SpriteHashEntry* next;
} THEntry;

class SpriteHash : public HashTable{
	public:
		SpriteHash(unsigned int size);
		~SpriteHash();

		void add(const char* key, SDL_Surface* surface);
		bool has(const char* key);
		SDL_Surface* get(const char* key);
	
	private:
		unsigned int size;
		std::atomic<THEntry**> table;
};
#endif