#ifndef MUSIC_H
#define MUSIC_H

#include <unordered_map>
#include <string>
#include <unordered_set>

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
        int serialize(FILE* file, std::unordered_set<std::string>& music_set);
        bool isPlaying();

    private:
        void fadeVolume(int channel_id, float volume, unsigned int fade);

        char* name;
        bool playing;
        int music_channel_index;
        uint16_t num_tracks;
        Sound* tracks[MAX_MUSIC_TRACKS];
};

#endif