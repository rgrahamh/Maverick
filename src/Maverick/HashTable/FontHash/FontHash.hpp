#ifndef FONTHASH_H
#define FONTHASH_H

#include <atomic>

#include "../HashTable.hpp"
#include "Maverick/Font/Font.hpp"

typedef struct FontHashEntry{
	char* key;
	Font* font;
	struct FontHashEntry* next;
} FHEntry;

class FontHash : public HashTable{
	public:
		FontHash(unsigned int size);
		~FontHash();

		void add(const char* key, Font* font);
		bool has(const char* key);
		Font* get(const char* key);
	
	private:
		std::atomic<FHEntry**> table;
};

#endif