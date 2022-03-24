#include "Music.hpp"
#include "../../Engine/Engine.hpp"
extern Engine* engine;

Music::Music(const char* name){
    this->playing = false;
    this->music_channel_index = 0;
    this->name = StrDeepCopy(name);
}

int Music::start(int music_channel_index, float volume, unsigned int fade){
    if(this->playing){
        return -1;
    }

    int channel_offset = music_channel_index * MAX_MUSIC_TRACKS;
    if(fade == 0){
        for(int i = 0; i < this->num_tracks; i++){
            Mix_PlayChannel(i + channel_offset, tracks[i]->sample, -1);
        }
    }
    else{
        this->music_channel_index = music_channel_index;
        int channel_offset = this->music_channel_index * MAX_MUSIC_TRACKS;
        if(!playing){
            for(int i = 0; i < this->num_tracks; i++){
                Mix_Volume(i + channel_offset, 0);
                Mix_PlayChannel(i + channel_offset, tracks[i]->sample, -1);
                std::thread(&Music::fadeVolume, this, i + channel_offset, volume, fade);
            }
        }
    }
    this->music_channel_index = music_channel_index;
    this->playing = true;

    return 0;
}

int Music::stop(unsigned int fade){
    if(!this->playing){
        return -1;
    }

    if(fade == 0){
        Mix_HaltGroup(this->music_channel_index + 1);
    }
    else{
        Mix_FadeOutGroup(this->music_channel_index + 1, fade);
    }
    this->playing = false;

    return 0;
}

int Music::pause(){
    if(!this->playing){
        return -1;
    }

    int channel_offset = this->music_channel_index * MAX_MUSIC_TRACKS;
    for(int i = 0; i < this->num_tracks; i++){
        Mix_Pause(i + channel_offset);
    }
    this->playing = false;

    return 0;
}

int Music::resume(){
    if(this->playing){
        return -1;
    }

    int channel_offset = this->music_channel_index * MAX_MUSIC_TRACKS;
    for(int i = 0; i < this->num_tracks; i++){
        Mix_Resume(i + channel_offset);
    }
    this->playing = false;

    return 0;
}

int Music::setInstrumentVolume(const char* instrument_name, float volume, unsigned int fade){
    int channel_offset = this->music_channel_index * MAX_MUSIC_TRACKS;
    for(int i = 0; i < this->num_tracks; i++){
        if(strcmp(tracks[i]->name, instrument_name) == 0){
            std::thread(&Music::fadeVolume, this, i + channel_offset, volume, fade);
            return 0;
        }
    }

    return -1;
}

void Music::setVolume(float volume, unsigned int fade){
    int channel_offset = this->music_channel_index * MAX_MUSIC_TRACKS;
    for(int i = 0; i < this->num_tracks; i++){
        if(fade == 0){
            Mix_Volume(i + channel_offset, volume);
        }
        else{
            std::thread(&Music::fadeVolume, this, i + channel_offset, volume, fade);
        }
    }
}

/** Fades the volume to a certain level (THIS IS BLOCKING, ONLY RUN THIS FROM SIDE THREADS)
 * @param channel_id The channel ID that we'd like to fade
 * @param volume The volume we'd like to fade to (0-1.0)
 * @param fade The number of ms we'd like to fade
 */
void Music::fadeVolume(int channel_id, float volume, unsigned int fade){
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

	engine->cleanupThread(std::this_thread::get_id());
}

int Music::addTrack(Sound* track){
    if(this->num_tracks >= MAX_MUSIC_TRACKS || track == nullptr){
        return -1;
    }

    this->tracks[this->num_tracks++] = track;
    return 0;
}

bool Music::isPlaying(){
    return this->playing;
}