#ifndef ATTRIBUTEHASH_HPP
#define ATTRIBUTEHASH_HPP

#include <string.h>
#include <SDL2/SDL.h>

#include "../../Global/Global.hpp"

//Don't use just the type name in all caps; that's reserved in Windowsese
enum ATTR_DATA_TYPE{
    DATA_INT = 0,
    DATA_UINT,
    DATA_BOOL,
    DATA_CHAR,
    DATA_STRING
};

typedef struct Attribute{
    ATTR_DATA_TYPE type;
	char* key;
	void* val;
	struct Attribute* next;
} Attribute;

class AttributeHash{
	public:
		AttributeHash(unsigned int size);
		~AttributeHash();

		void set(const char* key, void* val, ATTR_DATA_TYPE type);
		Attribute* get(const char* key);
		unsigned int dump(Attribute** hash_ptr);

	protected:
		unsigned int size;
		virtual unsigned int hash(const char* key);

	private:
		Attribute** table;
};
#endif