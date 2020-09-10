#include <SFML/Graphics.hpp>
#include <string.h>

typedef struct TextureHashEntry{
	char* key;
	sf::Texture* texture;
	struct TextureHashEntry* next;
} THEntry;

class TextureHash{
	public:
		TextureHash(unsigned int size);
		~TextureHash();

		void add(const char* key, sf::Texture* texture);
		sf::Texture* get(const char* key);
	
	private:
		unsigned int size;
		THEntry** table;

		unsigned int hash(const char* key);
};