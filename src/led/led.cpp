#define SDL_MAIN_HANDLED
#include "Maverick/Engine/Engine.hpp"
#include "Cyberena/Zones/global/global.hpp"
#include "Cyberena/Zones/TestZone/TestZone.hpp"
#include "Maverick/Audio/SoundBoard/SoundBoard.hpp"
#include "Zones/led/led.hpp"

int main(int argc, char** argv){
	Engine* engine = Engine::getInstance();

    buildGlobal();
	buildTestZone();
	buildEditor();

    Music* song1 = new Music("song1");
    Sound* sound = new Sound;
    sound->name = "bass";
    sound->sample = Mix_LoadWAV("./assets/audio/music/bass_riff_idea.wav");
    song1->addTrack(sound);
    engine->addMusic("song1", song1);
	SoundBoard* sound_board = engine->getSoundBoard();
    //sound_board->playMusic("song1");
    //sound_board->setMusicVolume(1, 1.0, 10000);

	engine->start();
	return 0;
}
