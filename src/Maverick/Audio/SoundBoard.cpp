#include "SoundBoard.hpp"

SoundBoard::SoundBoard(){
    Mix_AllocateChannels(NUM_CHANNELS);

    //Setting the "Music 1" channel group to 3
    Mix_GroupChannel(BASS_CHANNEL_1, 1);
    Mix_GroupChannel(LEAD_GUITAR_CHANNEL_1, 1);
    Mix_GroupChannel(RHYTHM_GUITAR_CHANNEL_1, 1);
    Mix_GroupChannel(DRUMS_CHANNEL_1, 1);
    Mix_GroupChannel(SYNTH_CHANNEL_1, 1);
    Mix_GroupChannel(MISC_CHANNEL_1, 1);

    //Setting the "Music 2" channel group to 3
    Mix_GroupChannel(BASS_CHANNEL_2, 2);
    Mix_GroupChannel(LEAD_GUITAR_CHANNEL_2, 2);
    Mix_GroupChannel(RHYTHM_GUITAR_CHANNEL_2, 2);
    Mix_GroupChannel(DRUMS_CHANNEL_2, 2);
    Mix_GroupChannel(SYNTH_CHANNEL_2, 2);
    Mix_GroupChannel(MISC_CHANNEL_2, 2);

    Mix_ReserveChannels(UNRESERVED_CHANNEL);
}

SoundBoard::~SoundBoard(){
}

void SoundBoard::setInstruments(Music* music, int channel_id, unsigned int fade){
    unsigned int channel_offset = (channel_id - 1) * 6;
    if(fade > 0){
        if(music->bass != nullptr){
            Mix_FadeInChannel(BASS_CHANNEL_1 + channel_offset, music->bass, -1, fade);
        }
        if(music->rhythm_guitar != nullptr){
            Mix_FadeInChannel(LEAD_GUITAR_CHANNEL_2 + channel_offset, music->lead_guitar, -1, fade);
        }
        if(music->lead_guitar != nullptr){
            Mix_FadeInChannel(RHYTHM_GUITAR_CHANNEL_2 + channel_offset, music->rhythm_guitar, -1, fade);
        }
        if(music->drums != nullptr){
            Mix_FadeInChannel(DRUMS_CHANNEL_2 + channel_offset, music->drums, -1, fade);
        }
        if(music->synth != nullptr){
            Mix_FadeInChannel(SYNTH_CHANNEL_2 + channel_offset, music->synth, -1, fade);
        }
        if(music->misc != nullptr){
            Mix_FadeInChannel(MISC_CHANNEL_2 + channel_offset, music->misc, -1, fade);
        }
    }
    else{
        if(music->bass != nullptr){
            Mix_PlayChannel(BASS_CHANNEL_1 + channel_offset, music->bass, -1);
        }
        if(music->rhythm_guitar != nullptr){
            Mix_PlayChannel(LEAD_GUITAR_CHANNEL_1 + channel_offset, music->lead_guitar, -1);
        }
        if(music->lead_guitar != nullptr){
            Mix_PlayChannel(RHYTHM_GUITAR_CHANNEL_1 + channel_offset, music->rhythm_guitar, -1);
        }
        if(music->drums != nullptr){
            Mix_PlayChannel(DRUMS_CHANNEL_1 + channel_offset, music->drums, -1);
        }
        if(music->synth != nullptr){
            Mix_PlayChannel(SYNTH_CHANNEL_1 + channel_offset, music->synth, -1);
        }
        if(music->misc != nullptr){
            Mix_PlayChannel(MISC_CHANNEL_1 + channel_offset, music->misc, -1);
        }
    }
}

/** Playing music
 * @param music A ptr to the Music struct, holding our info
 * @param fade Any fade you'd like to do (will blend the old & new music tracks automagically)
 */
int SoundBoard::playMusic(Music* music, unsigned int fade){
    bool ch1_playing = Mix_Playing(BASS_CHANNEL_1) | Mix_Playing(LEAD_GUITAR_CHANNEL_1) | Mix_Playing(RHYTHM_GUITAR_CHANNEL_1) | Mix_Playing(SYNTH_CHANNEL_1) | Mix_Playing(DRUMS_CHANNEL_1);
    bool ch2_playing = Mix_Playing(BASS_CHANNEL_2) | Mix_Playing(LEAD_GUITAR_CHANNEL_2) | Mix_Playing(RHYTHM_GUITAR_CHANNEL_2) | Mix_Playing(SYNTH_CHANNEL_2) | Mix_Playing(DRUMS_CHANNEL_2);

    //If channel 1 is playing anything
    if(ch1_playing){
        if(fade >= 0){
            //Fade out channel 1
            Mix_FadeOutGroup(1, fade);
        }
        else{
            //Stop channel 1
            Mix_HaltGroup(1);
        }

        //Start channel 2
        setInstruments(music, 2, fade);
    }
    //If channel 1 isn't playing anything (will hit both when nothing is playing & when just channel 2 is playing)
    else{
        if(ch2_playing){
            if(fade >= 0){
                //Fade out channel 2
                Mix_FadeOutGroup(2, fade);
            }
            else{
                //Stop channel 2
                Mix_HaltGroup(2);
            }
        }

        //Start channel 1
        setInstruments(music, 1, fade);
    }
}

/** Sets the channel volume
 * @param channel_id The channel to set the volume for (if -1, sets the volume for all channels)
 * @param volume The volume level (as a positive float; 1.0 is max volume)
 */
void SoundBoard::setChannelVolume(int channel_id, float volume, unsigned int fade){
    //No fade
    if(fade == 0){
        Mix_Volume(channel_id, MIX_MAX_VOLUME * volume);
    }
    else{
        //Fade to volume for selected channel
        if(channel_id != -1){
            std::thread fade_thread(&SoundBoard::fadeVolume, this, channel_id, volume, fade);
        }
        //Fade to volume for all playing channels
        else{
            for(int i = 0; i < NUM_CHANNELS; i++){
                if(Mix_Playing(i)){
                    std::thread fade_thread(&SoundBoard::fadeVolume, this, channel_id, volume, fade);
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
        Mix_Pause(BASS_CHANNEL_1);
        Mix_Pause(LEAD_GUITAR_CHANNEL_1);
        Mix_Pause(RHYTHM_GUITAR_CHANNEL_1);
        Mix_Pause(DRUMS_CHANNEL_1);
        Mix_Pause(SYNTH_CHANNEL_1);
        Mix_Pause(MISC_CHANNEL_1);
    }

    if(channel_id == -1 || channel_id == 2){
        Mix_Pause(BASS_CHANNEL_2);
        Mix_Pause(LEAD_GUITAR_CHANNEL_2);
        Mix_Pause(RHYTHM_GUITAR_CHANNEL_2);
        Mix_Pause(DRUMS_CHANNEL_2);
        Mix_Pause(SYNTH_CHANNEL_2);
        Mix_Pause(MISC_CHANNEL_2);
    }
}

/** Resumes music for the specified channel
 * @param channel_id The channel you wish to resume music for (-1 if you wish to resume all channels)
 */
void SoundBoard::resumeMusic(int channel_id){
    if(channel_id == -1 || channel_id == 1){
        Mix_Resume(BASS_CHANNEL_1);
        Mix_Resume(LEAD_GUITAR_CHANNEL_1);
        Mix_Resume(RHYTHM_GUITAR_CHANNEL_1);
        Mix_Resume(DRUMS_CHANNEL_1);
        Mix_Resume(SYNTH_CHANNEL_1);
        Mix_Resume(MISC_CHANNEL_1);
    }

    if(channel_id == -1 || channel_id == 2){
        Mix_Resume(BASS_CHANNEL_2);
        Mix_Resume(LEAD_GUITAR_CHANNEL_2);
        Mix_Resume(RHYTHM_GUITAR_CHANNEL_2);
        Mix_Resume(DRUMS_CHANNEL_2);
        Mix_Resume(SYNTH_CHANNEL_2);
        Mix_Resume(MISC_CHANNEL_2);
    }
}

/** Plays a sound <loops + 1> number of times
 * @param sound_chunk The sound chunk that we're playing
 * @param loops The number of loops we're playing
 */
int SoundBoard::playSound(Mix_Chunk* sound_chunk, int loops){
    if(sound_chunk == nullptr || Mix_PlayChannel(-1, sound_chunk, loops) == -1){
        return -1;
    }
    return 0;
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

    int8_t volume_diff = abs(starting_volume - target_volume);
    uint8_t volume_dir = (target_volume > starting_volume)? 1 : -1;
    for(int i = 0; i < volume_diff; i++){
        //In microseconds for more granularity
        std::this_thread::sleep_for(std::chrono::microseconds((fade * 1000) / volume_diff));
        current_volume += volume_dir;
        Mix_Volume(channel_id, current_volume);
    }
}