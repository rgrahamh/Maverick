#ifndef TEXTUREHASH_HPP
#define TEXTUREHASH_HPP

#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <atomic>

#include "../HashTable.hpp"

typedef struct TextureHashEntry{
	char* key;
	SDL_Surface* surface;
	struct TextureHashEntry* next;
} THEntry;

class TextureHash : public HashTable{
	public:
		TextureHash(unsigned int size);
		~TextureHash();

		void add(const char* key, SDL_Surface* surface);
		bool has(const char* key);
		SDL_Surface* get(const char* key);
	
	private:
		unsigned int size;
		std::atomic<THEntry**> table;
};
#endif