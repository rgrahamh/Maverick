#include "./AttributeHash.hpp"

/** The paramterized constructor for the AttributeHash
 * @param size The size of the hash table
 */
AttributeHash::AttributeHash(unsigned int size){
	this->size = size;
	this->table = (AHEntry**)calloc(sizeof(AHEntry*), size);
}

/** The destructor for the AttributeHash
 */
AttributeHash::~AttributeHash(){
	for(unsigned int i = 0; i < this->size; i++){
		if(this->table[i] != NULL){
			AHEntry* cursor = this->table[i];
			while(cursor != NULL){
				AHEntry* tmp = cursor;
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
unsigned int AttributeHash::hash(const char* key){
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
void AttributeHash::add(const char* key, void* val){
	unsigned int hash_val = this->hash(key);

	//Copying the key for permanent storage
	unsigned int len = strlen(key);
	char* perm_key = (char*)malloc(len + 1);
	strncpy(perm_key, key, len);
	perm_key[len] = '\0';

	//Storing in the table
	AHEntry* new_entry = (AHEntry*)malloc(sizeof(AHEntry));
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
void* AttributeHash::get(const char* key){
	unsigned int hash_val = this->hash(key);

	AHEntry* cursor = this->table[hash_val];

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

/** Iterates over & dumps the AttributeHash
 * @return A AttributeHashEntry ptr to a new list (MUST iterate over elements to delete them all)
 */
unsigned int AttributeHash::dump(AttributeHashEntry** hash_ptr){
	unsigned int entries = 0;
	AttributeHashEntry* ht_dump = nullptr;
	for(unsigned int i = 0; i < size; i++){
		while(table[i] != nullptr){
			if(ht_dump == nullptr){
				ht_dump = new AttributeHashEntry;
				ht_dump->next = nullptr;
			}
			else{
				AttributeHashEntry* ht_tmp = new AttributeHashEntry;
				ht_tmp->next = ht_dump;
				ht_dump = ht_tmp;
			}
			ht_dump->key = table[i]->key;
			ht_dump->val = table[i]->val;
            ht_dump->type = table[i]->type;

			entries++;
		}
	}

	*hash_ptr = ht_dump;

	return entries;
}