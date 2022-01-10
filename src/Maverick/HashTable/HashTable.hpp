#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <string.h>
#include <SDL2/SDL.h>

typedef struct HashTableEntry{
	char* key;
	void* val;
	struct HashTableEntry* next;
} HTEntry;

class HashTable{
	public:
		HashTable(unsigned int size);
		~HashTable();

		virtual void add(const char* key, void* val);
		virtual void* get(const char* key);

	protected:
		unsigned int size;
		virtual unsigned int hash(const char* key);

	private:
		HTEntry** table;
};
#endif