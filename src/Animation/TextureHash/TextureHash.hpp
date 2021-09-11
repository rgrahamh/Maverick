#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct TextureHashEntry{
	char* key;
	SDL_Surface* surface;
	struct TextureHashEntry* next;
} THEntry;

class TextureHash{
	public:
		TextureHash(unsigned int size);
		~TextureHash();

		void add(const char* key, SDL_Surface* surface);
		SDL_Surface* get(const char* key);
	
	private:
		unsigned int size;
		THEntry** table;

		unsigned int hash(const char* key);
};