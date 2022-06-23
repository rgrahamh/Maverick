#include "./AttributeHash.hpp"

/** The paramterized constructor for the AttributeHash
 * @param size The size of the hash table
 */
AttributeHash::AttributeHash(unsigned int size){
	this->size = size;
	this->table = (Attribute**)calloc(sizeof(Attribute*), size);
}

/** The destructor for the AttributeHash
 */
AttributeHash::~AttributeHash(){
	for(unsigned int i = 0; i < this->size; i++){
		if(this->table[i] != NULL){
			Attribute* cursor = this->table[i];
			while(cursor != NULL){
				Attribute* tmp = cursor;
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
 * @param val The attribute value
 * @param type The type of the value
 */
void AttributeHash::set(const char* key, void* val, ATTR_DATA_TYPE type){
	//Make a deep copy if it's a string
	if(type == ATTR_DATA_TYPE::DATA_STRING){
		val = StrDeepCopy((const char*)val);
	}

	//Add a new attribute if it doesn't exist, or overwrite the old value
	Attribute* entry = this->get(key);
	if(entry == nullptr){
		unsigned int hash_val = this->hash(key);

		//Copying the key for permanent storage
		unsigned int len = strlen(key);
		char* perm_key = (char*)malloc(len + 1);
		strncpy(perm_key, key, len);
		perm_key[len] = '\0';

		//Storing in the table
		Attribute* new_entry = (Attribute*)malloc(sizeof(Attribute));
		new_entry->key = perm_key;
		new_entry->val = val;
		new_entry->type = type;

		//Setting it as the first thing in the linked list (for constant-time insertion)
		if(table[hash_val] == NULL){
			new_entry->next = NULL;
		}
		else{
			new_entry->next = table[hash_val];
		}
		table[hash_val] = new_entry;
	}
	else{
		if(entry->type == ATTR_DATA_TYPE::DATA_STRING && entry->val != nullptr){
			free(entry->val);
		}
		entry->val = val;
		entry->type = type;
	}
}

/** Gets the texture from the hash table
 * @param key The string (filepath) that is being hashed
 * @return A pointer to the attribute entry, or nullptr of no such entry exists
 */
Attribute* AttributeHash::get(const char* key){
	unsigned int hash_val = this->hash(key);

	Attribute* cursor = this->table[hash_val];

	//Iterate until we hit a matching case
	while(cursor != NULL && strcmp(cursor->key, key) != 0){
		cursor = cursor->next;
	}

    if(cursor == NULL){
        return NULL;
    }
    else{
        return cursor;
    }
}

/** Iterates over & dumps the AttributeHash
 * @return A Attribute ptr to a new list (MUST iterate over elements to delete them all)
 */
unsigned int AttributeHash::dump(Attribute** hash_ptr){
	unsigned int entries = 0;
	Attribute* ht_dump = nullptr;
	for(unsigned int i = 0; i < size; i++){
		while(table[i] != nullptr){
			if(ht_dump == nullptr){
				ht_dump = new Attribute;
				ht_dump->next = nullptr;
			}
			else{
				Attribute* ht_tmp = new Attribute;
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