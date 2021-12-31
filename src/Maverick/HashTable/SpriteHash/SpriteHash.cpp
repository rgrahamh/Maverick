#include "./SpriteHash.hpp"

extern SDL_Renderer* renderer;

/** The paramterized constructor for the SpriteHash
 * @param size The size of the hash table
 */
SpriteHash::SpriteHash(unsigned int size) : HashTable(size){
	this->table = (THEntry**)calloc(sizeof(THEntry*), size);
}

/** The destructor for the SpriteHash
 */
SpriteHash::~SpriteHash(){
	for(unsigned int i = 0; i < this->size; i++){
		if(this->table[i] != NULL){
			THEntry* cursor = this->table[i];
			while(cursor != NULL){
				THEntry* tmp = cursor;
				cursor = cursor->next;

				free(tmp->key);
				free(tmp->surface);
				free(tmp);
			}
		}
	}
}

/** Adds a new entry to the hash table
 * @param key The string (filepath) that is being hashed
 * @param surface The texture that is being put into the table
 */
void SpriteHash::add(const char* key, SDL_Surface* surface){
	unsigned int hash_val = this->hash(key);

	//Copying the key for permanent storage
	unsigned int len = strlen(key);
	char* perm_key = (char*)malloc(len + 1);
	strncpy(perm_key, key, len);
	perm_key[len] = '\0';

	//Storing in the table
	THEntry* new_entry = (THEntry*)malloc(sizeof(THEntry));
	new_entry->key = perm_key;
	new_entry->surface = surface;

	//Setting it as the first thing in the linked list (for constant-time insertion)
	if(table[hash_val] == NULL){
		new_entry->next = NULL;
	}
	else{
		new_entry->next = table[hash_val];
	}
	table[hash_val] = new_entry;

	printf("Loaded surface %s\n", key);
}

/** Gets the texture from the hash table
 * @param key The string (filepath) that is being hashed
 * @return The texture with the given key
 */
SDL_Surface* SpriteHash::get(const char* key){
	unsigned int hash_val = this->hash(key);

	printf("Getting texture %s\n", key);

	THEntry* cursor = this->table[hash_val];

	//Iterate until we hit a matching case
	while(cursor != NULL && strcmp(cursor->key, key) != 0){
		cursor = cursor->next;
	}
	if(cursor == NULL){
		SDL_Surface* new_surface = IMG_Load(key);
		if(new_surface == nullptr){
			printf("Can't load the file: %s as a surface\n", key);
			return NULL;
		}
		add(key, new_surface);

		return new_surface;
	}

	return cursor->surface;
}