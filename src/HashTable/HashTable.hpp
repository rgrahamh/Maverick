#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct HashTableEntry{
	char* key;
	void* val;
	struct HashTableEntry* next;
} HTEntry;

class HashTable{
	public:
		HashTable(unsigned int size);
		~HashTable();

		void add(const char* key, void* val);
		void* get(const char* key);
	
	protected:
		unsigned int size;
		HTEntry** table;

		unsigned int hash(const char* key);
};
#endif