#include <SFML/Graphics.hpp>
#include <string.h>

typedef struct SpriteHashEntry{
	char* key;
	sf::Sprite* sprite;
	SHEntry* next;
} SHEntry;

class SpriteHash{
	public:
		SpriteHash(unsigned int size);
		~SpriteHash();

		void add(const char* key, sf::Sprite* sprite);
		sf::Sprite* get(const char* key);
	
	private:
		unsigned int size;
		SHEntry** table;

		unsigned int hash(const char* key);
};