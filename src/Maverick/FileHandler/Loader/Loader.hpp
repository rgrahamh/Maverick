//What the color of her eyes were, or her scars and how she got them

#ifndef LOADER_H
#define LOADER_H
#include "Maverick/Zone/Zone.hpp"
#include "Maverick/Font/Font.hpp"
#include "Maverick/Audio/Music/Music.hpp"
#include "Maverick/Global/Global.hpp"

/** Loads a zone from file
 * @param zone_name The name of the new zone to load
 * @return A pointer to the newly loaded zone
 */
Zone* loadZoneFromFile(const char* zone_name);

/** Loads a BMP
 * @param file The file to load the BMP from
 * @param key_buff The buffer to print the surface identifier to (to be passed back ByRef)
 * @param max_len The max len of key_buff
 * @return A pointer to the newly loaded surface
 */
SDL_Surface* loadBMP(FILE* file, char* key_buff = nullptr, unsigned int max_len = 0);

/** Loads a sound
 * @param file The file to load the sound from
 * @param key_buff The buffer to print the sound identifier to (to be passed back ByRef)
 * @param max_len The max len of key_buff
 * @return A pointer to the newly loaded sound
 */
Sound* loadSound(FILE* file, char* key_buff = nullptr, unsigned int max_len = 0);

/** Loads music
 * @param file The file to load the music from
 * @param key_buff The buffer to print the music identifier to (to be passed back ByRef)
 * @param max_len The max len of key_buff
 * @return A pointer to the newly loaded music
 */
Music* loadMusic(FILE* file, char* key_buff = nullptr, unsigned int max_len = 0);

/** Loads a font
 * @param file The file to load the font from
 * @param key_buff The buffer to print the font identifier to (to be passed back ByRef)
 * @param max_len The max len of key_buff
 * @return A pointer to the newly loaded font
 */
Font* loadFont(FILE* file, char* key_buff = nullptr, unsigned int max_len = 0);
#endif