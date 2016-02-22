#ifndef XRSOUNDMANAGER_H
#define XRSOUNDMANAGER_H

#include "../include/FMOD/fmod_errors.h"
#include "../include/FMOD/fmod_studio.hpp"
/**
* XRSoundManager
* XRSoundManager is the audio mixer implemented with FMOD studio
*
* @ Che-Yuan Liang
*/

namespace XRSoundManager 
{
	
	bool init();
	bool update();
	bool destroy();

	void FmodErrorCheck(FMOD_RESULT result);
	void createSound(char *filePath, FMOD::Sound **sound);
	void playSound(FMOD::Sound *sound, FMOD::Channel **channel);
	void setChannelPosition(FMOD_VECTOR position, FMOD_VECTOR velocity, FMOD::Channel *channel);
	void setListenerPosition(FMOD_VECTOR position, FMOD_VECTOR velocity, FMOD_VECTOR foward, FMOD_VECTOR up);
	
}


#endif