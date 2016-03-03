#ifndef OCULUSWRAPPER_H
#define OCULUSWRAPPER_H

#include "glm\glm.hpp"
#include "XREntity.h"

namespace OculusWrapper
{
	bool start(); //start should be called before init
	bool init();
	bool update(double time, XREntity* scene);
	bool destroy();


	glm::mat4 getWorld2Clip();
	glm::mat4 getWorld2View();
	glm::mat4 getPersProj();

}



#endif
