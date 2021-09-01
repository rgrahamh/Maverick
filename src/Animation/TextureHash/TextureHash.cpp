#include "./TextureHash.hpp"

extern SDL_Renderer* renderer;

//The instance of the texture hash table
TextureHash* texture_hash;

/** The paramterized constructor for the TextureHash
 * @param size The size of the hash table
 */
TextureHash::TextureHash(unsigned int size){
	this->size = size;
	this->table = (THEntry**)calloc(sizeof(THEntry*), size);
}

/** The destructor for the TextureHash
 */
TextureHash::~TextureHash(){
	for(unsigned int i = 0; i < this->size; i++){
		if(this->table[i] != NULL){
			THEntry* cursor = this->table[i];
			while(cursor != NULL){
				THEntry* tmp = cursor;
				cursor = cursor->next;

				free(cursor->key);
				free(cursor->surface);
				free(tmp);
			}
		}
	}
}

/** Generates a hash based on string contents
 * @param key The key that we are hashing
 * @return The hash
 */
unsigned int TextureHash::hash(const char* key){
	if(this->size == 0){
		return 0;
	}
	else{
		unsigned int hash = 0;
		for(int i = 0; key[i] != '\0'; i++){
			hash += key[i];
		}
		return hash % this->size;
	}
}

/** Adds a new entry to the hash table
 * @param key The string (filepath) that is being hashed
 * @param surface The texture that is being put into the table
 */
void TextureHash::add(const char* key, SDL_Surface* surface){
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
}

/** Gets the texture from the hash table
 * @param key The string (filepath) that is being hashed
 * @return The texture with the given key
 */
SDL_Surface* TextureHash::get(const char* key){
	unsigned int hash_val = this->hash(key);

	THEntry* cursor = this->table[hash_val];

	//Iterate until we hit a matching case
	if(cursor != NULL){
		if(strcmp(cursor->key, key)){
			return cursor->surface;
		}
		while(cursor->next != NULL && strcmp(cursor->next->key, key) != 0){
			cursor = cursor->next;
		}
	} 
	if(cursor == NULL || cursor->next == NULL){
		SDL_Surface* new_surface = SDL_LoadBMP(key);
		if(new_surface == nullptr){
			printf("Can't load the file: %s as a surface\n", key);
		}

		this->add(key, new_surface);
		return new_surface;
	}

	return cursor->surface;
}