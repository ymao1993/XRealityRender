#include "XREffect.h"

bool XREffect::init()
{
	this->initEffect();
	return true;
}

bool XREffect::update(double time)
{
	if (skeletonMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	this->updateEffect(time);
	if (skeletonMode) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	return true;
}

bool XREffect::destroy()
{
	this->destroyEffect();
	return true;
}