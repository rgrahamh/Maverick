#include "./SoundHash.hpp"

extern SDL_Renderer* renderer;

/** The paramterized constructor for the SoundHash
 * @param size The size of the hash table
 */
SoundHash::SoundHash(unsigned int size) : HashTable(size){
	this->table = (SHEntry**)calloc(sizeof(SHEntry*), size);
}

/** The destructor for the SoundHash
 */
SoundHash::~SoundHash(){
	for(unsigned int i = 0; i < this->size; i++){
		if(this->table[i] != NULL){
			SHEntry* cursor = this->table[i];
			while(cursor != NULL){
				SHEntry* tmp = cursor;
				cursor = cursor->next;

				free(tmp->key);
				free(tmp->sound);
				free(tmp);
			}
		}
	}
}

/** Adds a new entry to the hash table
 * @param key The string (filepath) that is being hashed
 * @param surface The texture that is being put into the table
 */
void SoundHash::add(const char* key, Sound* sound){
	unsigned int hash_val = this->hash(key);

	//Copying the key for permanent storage
	unsigned int len = strlen(key);
	char* perm_key = (char*)malloc(len + 1);
	strncpy(perm_key, key, len);
	perm_key[len] = '\0';

	//Storing in the table
	SHEntry* new_entry = (SHEntry*)malloc(sizeof(SHEntry));
	new_entry->key = perm_key;
	new_entry->sound = sound;

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
Sound* SoundHash::get(const char* key){
	unsigned int hash_val = this->hash(key);

	SHEntry* cursor = this->table[hash_val];

	//Iterate until we hit a matching case
	while(cursor != NULL && strcmp(cursor->key, key) != 0){
		cursor = cursor->next;
	}
	if(cursor == NULL){
		Mix_Chunk* chunk = Mix_LoadWAV(key);
		if(chunk == nullptr){
			printf("Can't load the file: %s as an audio file!\n", key);
			return NULL;
		}

		Sound* sound = new Sound;
		sound->sample = chunk;

		unsigned int len = strlen(key);
		char* name = (char*)malloc(len + 1);
		strncpy(name, key, len);
		name[len] = '\0';
		sound->name = name;

		add(key, sound);

		return sound;
	}

	return cursor->sound;
}