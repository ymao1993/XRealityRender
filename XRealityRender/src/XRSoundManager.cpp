#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <map>

#include "XRSoundManager.h"

#include "../include/FMOD/OculusFMODSpatializerSettings.h"

namespace XRSoundManager
{
	static std::map<char*, FMOD::Sound*> trackManager;
	static FMOD::Studio::System* system = NULL;
	static FMOD::System* lowLevelSystem = NULL;
	static FMOD::DSP *osp_dsp;

	void FmodErrorCheck(FMOD_RESULT result)        // this is an error handling function
	{                                              // for FMOD errors
		if (result != FMOD_OK)
		{
			printf("FMOD error! (%d) %s", result, FMOD_ErrorString(result));
			std::system("Pause");
			exit(-1);
		}
	}

	bool init()
	{
		// init studio system
		FmodErrorCheck(FMOD::Studio::System::create(&system));
		
		// init lowlevel system
		FmodErrorCheck(system->getLowLevelSystem(&lowLevelSystem));

		int sampleRate = 0;
		FmodErrorCheck(lowLevelSystem->getSoftwareFormat(&sampleRate, NULL, NULL));
		unsigned int bufferSize = 0;
		FmodErrorCheck(lowLevelSystem->getDSPBufferSize(&bufferSize, NULL));
		FmodErrorCheck(lowLevelSystem->setSoftwareFormat(sampleRate, FMOD_SPEAKERMODE_SURROUND, 2)); // 3D

		FmodErrorCheck(system->initialize(1000, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, NULL));

		// set distance units
		const float DISTANCEFACTOR = 1.0f;          // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.
		lowLevelSystem->set3DSettings(1.0f, DISTANCEFACTOR, 1.0f);

		return true;
	}
	
	// trackManager serves as cache for sound clips
	void createSound(char *filePath, FMOD::Sound **sound)
	{
		if (trackManager.find(filePath) == trackManager.end()) {
			FmodErrorCheck(lowLevelSystem->createSound(filePath, FMOD_3D|FMOD_LOOP_NORMAL, 0, sound));
			trackManager.insert({ filePath, *sound });
		}
		else
			*sound = trackManager.find(filePath)->second;
	}

	void playSound(FMOD::Sound *sound, FMOD::Channel **channel)
	{
		FmodErrorCheck(lowLevelSystem->playSound(sound, 0, false, channel));
	}

	void setChannelPosition(FMOD_VECTOR position, FMOD_VECTOR velocity, FMOD::Channel *channel)
	{
		FmodErrorCheck(channel->set3DAttributes(&position, &velocity));
	}

	void setListenerPosition(FMOD_VECTOR position, FMOD_VECTOR velocity, FMOD_VECTOR foward, FMOD_VECTOR up)
	{
		FmodErrorCheck(lowLevelSystem->set3DListenerAttributes(0, &position, &velocity, &foward, &up));
	}

	bool update()
	{
		FmodErrorCheck(lowLevelSystem->update());
		return true;
	}

	bool destroy()
	{
		FmodErrorCheck(lowLevelSystem->close());
		FmodErrorCheck(lowLevelSystem->release());
		return true;
	}
}