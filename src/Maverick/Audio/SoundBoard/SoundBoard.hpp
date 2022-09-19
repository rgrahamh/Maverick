#ifndef SOUNDBOARD_H
#define SOUNDBOARD_H
#include <thread>
#include <atomic>

#include "../Audio.hpp"
#include "../Music/Music.hpp"

class SoundBoard{
    public:
        SoundBoard();
        ~SoundBoard();

        int playMusic(const char* music, unsigned int fade = 0.0);
        void pauseMusic(int channel_id);
        void resumeMusic(int channel_id);
        void stopMusic(int channel);

        void playEnvironment(const char* sound_chunk, int loops = 0);
        void stopEnvironment();
    
        int playSound(Sound* sound, int loops = 0, float left_pan = 1.0, float right_pan = 1.0);
        int stopSound(int channel_num);

        void setMusicVolume(int channel_id, float volume, unsigned int fade = 0);
        void setSoundVolume(int channel_id, float volume, unsigned int fade = 0);

    private:
        float music_level;
        float sound_level;

        void fadeVolume(unsigned int channel_id, float tagret_volume, unsigned int fade);

        Music* music[MAX_MUSIC_CHANNELS];

};
#endif