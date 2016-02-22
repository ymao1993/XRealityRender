#ifndef XRSOUND_H
#define XRSOUND_H

#include "XREntity.h"
#include "XRComponent.h"
#include "XRComponentType.h"
#include "../include/FMOD/fmod.hpp"
#include <map>

/**
 * XRSound
 * XRSound manage the sounds of the object, render the 3D audio 
 * given the object and listener position
 * 
 * @ Che-Yuan Liang 
 */


class XRSound : public XRComponent
{
private:	
	std::map<char*, FMOD::Sound*> soundClips;
	FMOD::Channel *channel;
	FMOD_VECTOR position;
	FMOD_VECTOR velocity;

public:
	XRSound() 
		:XRComponent(XR_COMPONENT_SOUND){}

	virtual bool init();
	virtual bool update(double time);
	virtual bool destroy();
	void loadSound(char* soundName, char* filePath);
	void playSound(char* soundName);
	

};

#endif