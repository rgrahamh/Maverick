#ifndef MUSIC_H
#define MUSIC_H

#include <unordered_map>
#include <string>

#include "../Audio.hpp"

class Music{
    public:
        Music(const char* name);
        int start(int music_channel_index, float volume = 1.0, unsigned int fade = 0);
        int stop(unsigned int fade = 0);
        int resume();
        int pause();
        void setVolume(float volume, unsigned int fade = 0);
        int setInstrumentVolume(const char* instrument_name, float volume, unsigned int fade = 0);
        int addTrack(Sound* track);
        
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