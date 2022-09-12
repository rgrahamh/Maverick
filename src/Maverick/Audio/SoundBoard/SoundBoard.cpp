#include "SoundBoard.hpp"
#include "Maverick/Engine/Engine.hpp"
#include "Maverick/Audio/Music/Music.hpp"

SoundBoard::SoundBoard(){
    Mix_AllocateChannels(NUM_CHANNELS);

    for(int i = 0; i < MAX_MUSIC_CHANNELS; i++){
        int start_channel = i * MAX_MUSIC_CHANNELS;
        Mix_GroupChannels(start_channel, start_channel + MAX_MUSIC_CHANNELS - 1, i);
    }

    Mix_ReserveChannels(MAX_MUSIC_CHANNELS * MAX_MUSIC_TRACKS);

    music_level = 1.0;
    sound_level = 1.0;

    memset(this->music, 0, sizeof(Music*) * MAX_MUSIC_CHANNELS);
}

SoundBoard::~SoundBoard(){
}

/** Playing music
 * @param music A ptr to the Music struct, holding our info
 * @param fade Any fade you'd like to do (will blend the old & new music tracks automagically)
 */
int SoundBoard::playMusic(const char* music, unsigned int fade){
    Music* new_music = Engine::getInstance()->getMusic(music);
    if(new_music == nullptr){
        return -1;
    }

    //If music[0] hasn't been initialized
    if(this->music[0] == nullptr){
        this->music[0] = new_music;
        this->music[0]->start(0, -1);
    }
    //If channel 1 is playing anything
    else if(this->music[0]->isPlaying()){
        this->music[0]->stop(fade);
        this->music[1] = new_music;
        this->music[1]->start(1, 1.0, fade);
    }
    //If channel 1 isn't playing anything (will hit both when nothing is playing & when just channel 2 is playing)
    else{
        if(this->music[1] != nullptr && this->music[1]->isPlaying()){
            this->music[1]->stop(fade);
        }
        this->music[0] = new_music;
        this->music[0]->start(1, 1.0, fade);
    }

    return 0;
}

void SoundBoard::setMusicVolume(int channel_id, float volume, unsigned int fade){
    if(channel_id == -1 || channel_id == 1){
        music[0]->setVolume(volume);
    }

    if(channel_id == -1 || channel_id == 2){
        music[1]->setVolume(volume);
    }
}

/** Sets the channel volume
 * @param channel_id The channel to set the volume for (if -1, sets the volume for all channels)
 * @param volume The volume level (as a positive float; 1.0 is max volume)
 */
void SoundBoard::setSoundVolume(int channel_id, float volume, unsigned int fade){
    //No fade
    if(fade == 0){
        Mix_Volume(channel_id, volume * MIX_MAX_VOLUME );
    }
    else{
        //Fade to volume for selected channel
        Engine* engine = Engine::getInstance();
        if(channel_id != -1){
            std::thread(&SoundBoard::fadeVolume, this, channel_id, volume, fade).detach();
        }
        //Fade to volume for all playing channels
        else{
            for(int i = 0; i < NUM_CHANNELS; i++){
                if(Mix_Playing(i)){
                    std::thread(&SoundBoard::fadeVolume, this, i, volume, fade).detach();
                }
            }
        }
    }
}

/** Pauses music for the specified channel
 * @param channel_id The channel you wish to pause music for (-1 if you wish to pause all channels)
 */
void SoundBoard::pauseMusic(int channel_id){
    if(channel_id == -1 || channel_id == 1){
        music[0]->pause();
    }

    if(channel_id == -1 || channel_id == 2){
        music[1]->pause();
    }
}

/** Resumes music for the specified channel
 * @param channel_id The channel you wish to resume music for (-1 if you wish to resume all channels)
 */
void SoundBoard::resumeMusic(int channel_id){
    if(channel_id == -1 || channel_id == 1){
        music[0]->resume();
    }

    if(channel_id == -1 || channel_id == 2){
        music[1]->resume();
    }
}

/** Resumes music for the specified channel
 * @param channel_id The channel you wish to stop music for (-1 if you wish to resume all channels)
 */
void SoundBoard::stopMusic(int channel_id){
    if(channel_id == -1 || channel_id == 1){
        music[0]->stop();
    }

    if(channel_id == -1 || channel_id == 2){
        music[1]->stop();
    }
}

/** Plays a sound <loops + 1> number of times
 * @param sound_chunk The sound chunk that we're playing
 * @param loops The number of loops we're playing
 */
int SoundBoard::playSound(Sound* sound, int loops, float left_pan, float right_pan){
    int channel_id = Mix_PlayChannel(-1, sound->sample, loops);
    if(channel_id != -1){
        Mix_Volume(channel_id, sound_level * MIX_MAX_VOLUME);
        Mix_SetPanning(channel_id, left_pan * MIX_MAX_VOLUME, right_pan * MIX_MAX_VOLUME);
    }

    return channel_id;
}

/** Stops a sound
 * @param channel_num The channel number we're stopping
 */
int SoundBoard::stopSound(int channel_num){
    return Mix_HaltChannel(channel_num);
}

/** Fades the volume to a certain level (THIS IS BLOCKING, ONLY RUN THIS FROM SIDE THREADS)
 * @param channel_id The channel ID that we'd like to fade
 * @param volume The volume we'd like to fade to (0-1.0)
 * @param fade The number of ms we'd like to fade
 */
void SoundBoard::fadeVolume(unsigned int channel_id, float volume, unsigned int fade){
    uint8_t starting_volume = Mix_Volume(channel_id, -1);
    uint8_t current_volume = starting_volume;
    uint8_t target_volume = volume * MIX_MAX_VOLUME;

    uint8_t volume_diff = abs(starting_volume - target_volume);
    int8_t volume_dir = (target_volume > starting_volume)? 1 : -1;
    for(int i = 0; i < volume_diff; i++){
        //In microseconds for more granularity
        std::this_thread::sleep_for(std::chrono::microseconds((fade * 1000) / volume_diff));
        current_volume += volume_dir;
        Mix_Volume(channel_id, current_volume);
    }
}