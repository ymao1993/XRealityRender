#ifndef ROTATINGTRIANGLE_H
#define ROTATINGTRIANGLE_H

#include "XREntity.h"

class RotatingTriangle :public XREntity
{
public:
	virtual bool init();
	virtual bool update(float time);
	virtual bool destroy();
};


#endif