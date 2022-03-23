#ifndef MUSIC_H
#define MUSIC_H

#include <unordered_map>
#include <string>

#include "../Audio.hpp"

class Music{
    public:
        Music(const char* name);
        int start(int music_channel_index, int loops);
        int stop();
        int resume();
        int pause();
        int fadeIn(int music_channel_index, float volume, unsigned int fade);
        int fadeOut(unsigned int fade);
        int setVolume(float volume, unsigned int fade);
        int setInstrumentVolume(const char* instrument_name, float volume, unsigned int fade);
        int addTrack(Sound* track);
        int setVolume(float volume);
        
        bool isPlaying();
    private:
        char* name;
        bool playing;
        int music_channel_index;
        void fadeVolume(int channel_id, float volume, unsigned int fade);

        Sound* tracks[MAX_MUSIC_TRACKS];
        int num_tracks;
};

#endif