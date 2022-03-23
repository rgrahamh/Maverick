#include "./MusicHash.hpp"

extern SDL_Renderer* renderer;

/** The paramterized constructor for the MusicHash
 * @param size The size of the hash table
 */
MusicHash::MusicHash(unsigned int size) : HashTable(size){
	this->table = (MHEntry**)calloc(sizeof(MHEntry*), size);
}

/** The destructor for the MusicHash
 */
MusicHash::~MusicHash(){
	for(unsigned int i = 0; i < this->size; i++){
		if(this->table[i] != NULL){
			MHEntry* cursor = this->table[i];
			while(cursor != NULL){
				MHEntry* tmp = cursor;
				cursor = cursor->next;

				free(tmp->key);
				free(tmp->music);
				free(tmp);
			}
		}
	}
}

/** Adds a new entry to the hash table
 * @param key The string (filepath) that is being hashed
 * @param surface The texture that is being put into the table
 */
void MusicHash::add(const char* key, Music* music){
	unsigned int hash_val = this->hash(key);

	//Copying the key for permanent storage
	unsigned int len = strlen(key);
	char* perm_key = (char*)malloc(len + 1);
	strncpy(perm_key, key, len);
	perm_key[len] = '\0';

	//Storing in the table
	MHEntry* new_entry = (MHEntry*)malloc(sizeof(MHEntry));
	new_entry->key = perm_key;
	new_entry->music = music;

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
Music* MusicHash::get(const char* key){
	unsigned int hash_val = this->hash(key);

	MHEntry* cursor = this->table[hash_val];

	//Iterate until we hit a matching case
	while(cursor != NULL && strcmp(cursor->key, key) != 0){
		cursor = cursor->next;
	}
	if(cursor == NULL){
		return nullptr;
	}

	return cursor->music;
}