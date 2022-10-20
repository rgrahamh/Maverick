#include "../Global/Global.hpp"
#include "../Engine/Engine.hpp"
#include "Maverick/FileHandler/FileHandler.hpp"

int SerializeSound(FILE* file, Sound* sound){
    if(file == nullptr || sound == nullptr){
        return -1;
    }

    uint8_t asset_type = RESOURCE_TYPE::SOUND;
    fwrite(&asset_type, 1, 1, file);

    //Identifier
    uint16_t identifier_len = strlen(sound->name);
    WriteVar(identifier_len, file);
    fwrite(sound->name, 1, identifier_len, file);

    //Writing the audio buffer (from the mixer chunk) to file
    WriteVar(sound->sample->alen, file);
    fwrite(sound->sample->abuf, 1, sound->sample->alen, file);

    return 0;
}