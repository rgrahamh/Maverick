#include "../Utility/Utility.hpp"
#include "../Engine/Engine.hpp"

int SerializeSound(FILE* file, Sound* sound){
    if(file == nullptr || sound == nullptr){
        return -1;
    }

    //Identifier len
    uint16_t identifier_len = strlen(sound->name);
    uint16_t identifier_len_swapped = EndianSwap(&identifier_len);

    uint8_t asset_type = RESOURCE_TYPE::SOUND;
    fwrite(&asset_type, 1, 1, file);

    //Identifier
    fwrite(&identifier_len_swapped, 2, 1, file);
    fwrite(sound->name, 1, identifier_len, file);

    //Writing the audio buffer (from the mixer chunk) to file
    uint32_t audio_len = EndianSwap(&sound->sample->alen);
    fwrite(&audio_len, sizeof(audio_len), 1, file);
    fwrite(sound->sample->abuf, 1, sound->sample->alen, file);

    return 0;
}