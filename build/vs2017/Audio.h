#pragma once

#define THEME "themeSong.wav"
#define MAX_VOLUME 1.0f

#include <audio\audio_manager.h>
#include <system/platform.h>
#include <assert.h>
namespace gef
{
	class Platform;
	class AudioManager;
}

class Audio
{
public:
	Audio(gef::Platform &);
	~Audio();

	void setAudioLevel(float);
	void setMusicPlaying(bool);

private:
	gef::AudioManager *audioManager;
	float volumeLevel;
};

