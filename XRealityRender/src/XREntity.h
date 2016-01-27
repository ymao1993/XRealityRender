#ifndef XRENTITY_H
#define XRENTITY_H

#include "XREngine.h"

class XREntity
{
public:
	virtual bool init();
	virtual bool update(float time);
	virtual bool destroy();
};


#endif