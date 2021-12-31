#ifndef SOUNDBOARD_H
#define SOUNDBOARD_H
#include <SDL2/SDL_mixer.h>
#include <thread>

#define NUM_CHANNELS 48

enum RESERVED_CHANNELS{
    //Music "channel" 1
    BASS_CHANNEL_1 = 0,
    LEAD_GUITAR_CHANNEL_1 = 1,
    RHYTHM_GUITAR_CHANNEL_1 = 2,
    DRUMS_CHANNEL_1 = 3,
    SYNTH_CHANNEL_1 = 4,
    MISC_CHANNEL_1 = 5,

    //Music "channel" 2
    BASS_CHANNEL_2 = 6,
    LEAD_GUITAR_CHANNEL_2 = 7,
    RHYTHM_GUITAR_CHANNEL_2 = 8,
    DRUMS_CHANNEL_2 = 9,
    SYNTH_CHANNEL_2 = 10,
    MISC_CHANNEL_2 = 11,

    //Start of the unreserved channels
    UNRESERVED_CHANNEL = 12
};

struct Music{
    const char* name = nullptr;
    Mix_Chunk* bass = nullptr;
    Mix_Chunk* lead_guitar = nullptr;
    Mix_Chunk* rhythm_guitar = nullptr;
    Mix_Chunk* drums = nullptr;
    Mix_Chunk* synth = nullptr;
    Mix_Chunk* misc = nullptr;
};

struct Sound{
    const char* name = nullptr;
    Mix_Chunk* sample = nullptr;
};

class SoundBoard{
    public:
        SoundBoard();
        ~SoundBoard();

        int playMusic(Music* music, unsigned int fade = 0.0);
        void pauseMusic(int channel_id);
        void resumeMusic(int channel_id);
        void stopMusic(int channel);

        void playEnvironment(Mix_Chunk* sound_chunk, int loops = 0);
        void stopEnvironment();
    
        int playSound(Mix_Chunk* sound_chunk, int loops = 0, float left_pan = 1.0, float right_pan = 1.0);
        int stopSound(int channel_num);

        void setChannelVolume(int channel_id, float volume, unsigned int fade = 0);

    private:
        float music_level;
        float sound_level;

        void fadeVolume(unsigned int channel_id, float tagret_volume, unsigned int fade);
        void setInstruments(Music* music, int channel_id, unsigned int fade = 0.0);

};
#endif