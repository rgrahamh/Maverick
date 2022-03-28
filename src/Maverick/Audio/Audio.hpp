#ifndef AUDIO_H
#define AUDIO_H
#include <SDL2/SDL_mixer.h>

//The total number of sound channels (max normal sounds + (MAX_MUSIC_CHANNELS * MAX_MUSIC_TRACKS))
#define NUM_CHANNELS 64

//The total number of music channels will be assumed to be 2
#define MAX_MUSIC_CHANNELS 2
//MAX_MUSIC_TRACKS can change, but make sure to bump NUM_CHANNELS accordingly (and leave room for sound)
#define MAX_MUSIC_TRACKS 16

struct Sound{
    const char* name = nullptr;
    Mix_Chunk* sample = nullptr;
};

int SerializeSound(FILE* file, Sound* sound);

#endif