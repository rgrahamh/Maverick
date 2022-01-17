#ifndef ATTRIBUTEHASH_HPP
#define ATTRIBUTEHASH_HPP

#include <string.h>
#include <SDL2/SDL.h>

enum ATTR_DATA_TYPE{
    INT_PTR = 0,
    UINT_PTR,
    BOOL_PTR,
    CHAR_PTR,
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