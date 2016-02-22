#include "XREffect.h"

bool XREffect::init()
{
	this->initEffect();
	return true;
}

bool XREffect::update(double time)
{
	this->updateEffect(time);
	return true;
}

bool XREffect::destroy()
{
	this->destroyEffect();
	return true;
}