#include "./HashTable.hpp"

/** The paramterized constructor for the HashTable
 * @param size The size of the hash table
 */
HashTable::HashTable(unsigned int size){
	this->size = size;
	this->table = (HTEntry**)calloc(sizeof(HTEntry*), size);
}

/** The destructor for the HashTable
 */
HashTable::~HashTable(){
	for(unsigned int i = 0; i < this->size; i++){
		if(this->table[i] != NULL){
			HTEntry* cursor = this->table[i];
			while(cursor != NULL){
				HTEntry* tmp = cursor;
				cursor = cursor->next;

				free(tmp->key);
				free(tmp);
			}
		}
	}
}

/** Generates a hash based on string contents
 * @param key The key that we are hashing
 * @return The hash
 */
unsigned int HashTable::hash(const char* key){
    unsigned int hash = 0;
    for(int i = 0; key[i] != '\0'; i++){
        hash += key[i];
    }
    return hash % this->size;
}

/** Adds a new entry to the hash table
 * @param key The string (filepath) that is being hashed
 * @param surface The texture that is being put into the table
 */
void HashTable::add(const char* key, void* val){
	unsigned int hash_val = this->hash(key);

	//Copying the key for permanent storage
	unsigned int len = strlen(key);
	char* perm_key = (char*)malloc(len + 1);
	strncpy(perm_key, key, len);
	perm_key[len] = '\0';

	//Storing in the table
	HTEntry* new_entry = (HTEntry*)malloc(sizeof(HTEntry));
	new_entry->key = perm_key;
	new_entry->val = val;

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
void* HashTable::get(const char* key){
	unsigned int hash_val = this->hash(key);

	HTEntry* cursor = this->table[hash_val];

	//Iterate until we hit a matching case
	while(cursor != NULL && strcmp(cursor->key, key) != 0){
		cursor = cursor->next;
	}

    if(cursor == NULL){
        return NULL;
    }
    else{
        return &(cursor->val);
    }
}