#include "XRCommon.h"
#include <map>
#include "XRSound.h"
#include "XRSoundManager.h"
#include "XRObject.h"
#include "utils\misc.h"

bool XRSound::init()
{
	return true;
}

void XRSound::loadSound(char *soundName, char *filePath) 
{
	FMOD::Sound *sound;
	XRSoundManager::createSound(filePath, &sound);
	soundClips.insert({soundName, sound});
}

void XRSound::playSound(char *soundName)
{
	FMOD::Sound *sound = 0;
	if (soundClips.find(soundName) != soundClips.end()) {
		sound = soundClips.find(soundName)->second;
		XRSoundManager::playSound(sound, &channel);
	}
	else
		XRDebug::log("audio file not found");
}

bool XRSound::update(double time)
{
	//fetch position info from obejct
	position = glmToFmod(this->object->transform.position);

	//TODO: compute velocity
	//...

	XRSoundManager::setChannelPosition(position, velocity, channel);
	XRSoundManager::update();
	return true;
}


bool XRSound::destroy()
{
	return true;
}
