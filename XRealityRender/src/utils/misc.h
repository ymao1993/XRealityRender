#ifndef MISC_H
#define MISC_H

#include "FMOD\fmod.h"
#include "glm\glm.hpp"

static inline FMOD_VECTOR glmToFmod(glm::vec3 glmVector)
{
	FMOD_VECTOR fmodVector = { glmVector[0], glmVector[1], glmVector[2] };
	return fmodVector;
}

#endif