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
		Music* new_music = loadMusic(key);
		if(new_music == nullptr){
			printf("Can't load the file: %s as a surface\n", key);
			return NULL;
		}
		add(key, new_music);

		return new_music;
	}

	return cursor->music;
}

/** Tries to load music based on a pattern
 * @param music_pattern The pattern string to build paths from
 * @return A pointer to the new Music; instruments that failed to load are nullptr. If no music was loaded, we return nullptr.
 */
Music* MusicHash::loadMusic(const char* music_pattern){
	int music_pattern_len = strlen(music_pattern);
	//Pattern len + longest pattern extension ("rhythm_guitar.wav") + 1
	char music_file[music_pattern_len + 19];
	memcpy(music_file, music_pattern, music_pattern_len);
	char* music_offset = music_file + music_pattern_len;

	Music* music = new Music;
	memcpy(music_offset, "_bass.wav", 10);
	music->bass = Mix_LoadWAV(music_pattern);

	memcpy(music_offset, "_lead_guitar.wav", 17);
	music->lead_guitar = Mix_LoadWAV(music_pattern);

	memcpy(music_offset, "_rhythm_guitar.wav", 19);
	music->rhythm_guitar = Mix_LoadWAV(music_pattern);

	memcpy(music_offset, "_drums.wav", 11);
	music->drums = Mix_LoadWAV(music_pattern);

	memcpy(music_offset, "_synth.wav", 11);
	music->synth = Mix_LoadWAV(music_pattern);

	memcpy(music_offset, "_misc.wav", 10);
	music->misc = Mix_LoadWAV(music_pattern);

	if(music->bass == nullptr && music->lead_guitar == nullptr && music->rhythm_guitar == nullptr && music->drums == nullptr && music->synth == nullptr && music->misc == nullptr){
		return nullptr;
	}

	//Copying the key for permanent storage
	unsigned int len = strlen(music_pattern);
	char* name = (char*)malloc(len + 1);
	strncpy(name, music_pattern, len);
	name[len] = '\0';

	music->name = name;

	return music;
}