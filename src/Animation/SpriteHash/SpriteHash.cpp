#include "./SpriteHash.hpp"

//The instance of the sprite hash table
SpriteHash* sprite_hash;

/** The paramterized constructor for the SpriteHash
 * @param size The size of the hash table
 */
SpriteHash::SpriteHash(unsigned int size){
	this->size = size;
	this->table = (SHEntry**)calloc(sizeof(SHEntry*), size);
}

/** The destructor for the SpriteHash
 */
SpriteHash::~SpriteHash(){
	for(int i = 0; i < this->size; i++){
		if(table[i] != NULL){
			SHEntry* cursor = table[i];
			while(cursor != NULL){
				SHEntry* tmp = cursor;
				cursor = cursor->next;

				free(cursor->key);
				free(cursor->sprite);
				free(tmp);
			}
		}
	}
}

/** Generates a hash based on string contents
 * @param key The key that we are hashing
 * @return The hash
 */
unsigned int SpriteHash::hash(const char* key){
	unsigned int hash = 0;
	for(int i = 0; key[i] != '\0'; i++){
		hash += key[i];
	}
	return hash % this->size;
}

/** Adds a new entry to the hash table
 * @param key The string (filepath) that is being hashed
 * @param sprite The sprite that is being put into the table
 */
void SpriteHash::add(const char* key, sf::Sprite* sprite){
	unsigned int hash_val = this->hash(key);

	//Copying the key for permanent storage
	unsigned int len = strlen(key);
	char* perm_key = (char*)malloc(len + 1);
	strcpy(perm_key, key);
	perm_key[len] = '\0';

	//Storing in the table
	SHEntry* new_entry = (SHEntry*)malloc(sizeof(SHEntry));
	new_entry->key = perm_key;
	new_entry->sprite = sprite;

	//Setting it as the first thing in the linked list (for constant-time insertion)
	if(table[hash_val] == NULL){
		new_entry->next = NULL;
	}
	else{
		new_entry->next = table[hash_val];
	}
	table[hash_val] = new_entry;
}

/** Gets the sprite from the hash table
 * @param key The string (filepath) that is being hashed
 * @return The sprite with the given key
 */
sf::Sprite* SpriteHash::get(const char* key){
	unsigned int hash_val = this->hash(key);

	SHEntry* cursor = table[hash_val];
	if(cursor == NULL){
		return NULL;
	}

	//Iterate until we hit a matching case
	while(cursor != NULL && strcmp(cursor->key, key) != 0){
		cursor = cursor->next;
	}

	return cursor->sprite;
}