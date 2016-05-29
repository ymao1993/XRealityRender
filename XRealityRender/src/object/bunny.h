#ifndef BUNNY_H
#define BUNNY_H

#include "../XRObject.h"
#include "../shader/PhongLightingGS.h"

class Bunny :public XRObject
{
public:

	Bunny() :XRObject("Bunny"){}

	/*extended from XREntity*/
	virtual bool initObject();
	virtual bool updateObject(double time);
	virtual bool destroyObject();

};


#endif