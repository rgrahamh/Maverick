#ifndef ATTRIBUTEHASH_HPP
#define ATTRIBUTEHASH_HPP

#include <string.h>
#include <SDL2/SDL.h>

enum ATTR_DATA_TYPE{
    INT = 0,
    UINT,
    BOOL,
    CHAR,
    STRING
};

typedef struct AttributeHashEntry{
    ATTR_DATA_TYPE type;
	char* key;
	void* val;
	struct AttributeHashEntry* next;
} AHEntry;

class AttributeHash{
	public:
		AttributeHash(unsigned int size);
		~AttributeHash();

		virtual void add(const char* key, void* val);
		virtual void* get(const char* key);
		unsigned int dump(AttributeHashEntry** hash_ptr);

	protected:
		unsigned int size;
		virtual unsigned int hash(const char* key);

	private:
		AHEntry** table;
};
#endif