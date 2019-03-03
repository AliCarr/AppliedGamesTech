#include "Audio.h"

Audio::Audio(gef::Platform &platform)
{
	//Initialise all of the audio managers settngs,
	//and make sure the music isn't playing straight away.
	audioManager = gef::AudioManager::Create();
	audioManager->LoadMusic(THEME, platform);
	audioManager->SetMasterVolume(MAX_VOLUME);
	audioManager->StopMusic();

	assert(audioManager);
}


Audio::~Audio()
{
	delete audioManager;
	audioManager = 0;
}

void Audio::setAudioLevel(float level)
{
	audioManager->SetMasterVolume(level);
}

void Audio::setMusicPlaying(bool isPlaying)
{
	//Allow the main application to stop and start music
	if(!isPlaying)
		audioManager->StopMusic();

	if (isPlaying)
		audioManager->PlayMusic();
}
