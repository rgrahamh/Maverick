#include "./FontHash.hpp"

/** The paramterized constructor for the FontHash
 * @param size The size of the hash table
 */
FontHash::FontHash(unsigned int size) : HashTable(size){
	this->table = (FHEntry**)calloc(sizeof(FHEntry*), size);
}

/** The destructor for the FontHash
 */
FontHash::~FontHash(){
	for(unsigned int i = 0; i < this->size; i++){
		if(this->table[i] != NULL){
			FHEntry* cursor = this->table[i];
			while(cursor != NULL){
				FHEntry* tmp = cursor;
				cursor = cursor->next;

				free(tmp->key);
				free(tmp->font);
				free(tmp);
			}
		}
	}
}

/** Adds a new entry to the hash table
 * @param key The string (filepath) that is being hashed
 * @param surface The texture that is being put into the table
 */
void FontHash::add(const char* key, Font* font){
	unsigned int hash_val = this->hash(key);

	//Copying the key for permanent storage
	unsigned int len = strlen(key);
	char* perm_key = (char*)malloc(len + 1);
	strncpy(perm_key, key, len);
	perm_key[len] = '\0';

	//Storing in the table
	FHEntry* new_entry = (FHEntry*)malloc(sizeof(FHEntry));
	new_entry->key = perm_key;
	new_entry->font = font;

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
Font* FontHash::get(const char* key){
	unsigned int hash_val = this->hash(key);

	FHEntry* cursor = this->table[hash_val];

	//Iterate until we hit a matching case
	while(cursor != NULL && strcmp(cursor->key, key) != 0){
		cursor = cursor->next;
	}
	if(cursor == NULL){
		return nullptr;
	}

	return cursor->font;
}