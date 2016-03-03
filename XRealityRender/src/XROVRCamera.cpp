#include "XROVRCamera.h"
#include "XRSoundManager.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include "XREngine.h"
#include "utils\misc.h"

#include "OculusWrapper.h"

bool XROVRCamera::init()
{
	//OculusWrapper::init();
	return true;
}

bool XROVRCamera::update(double time)
{
	OculusWrapper::update(time, scene);
	return true;
}

void XROVRCamera::setScene(XRScene* scene)
{
	this->scene = scene;
}

bool XROVRCamera::destroy()
{
	OculusWrapper::destroy();
	return true;
}

glm::mat4 XROVRCamera::getWorld2Clip()
{
	return OculusWrapper::getWorld2Clip();
}

glm::mat4 XROVRCamera::getWorld2View()
{
	return OculusWrapper::getWorld2View();
}


glm::mat4 XROVRCamera::getPersProj()
{
	return OculusWrapper::getPersProj();
}