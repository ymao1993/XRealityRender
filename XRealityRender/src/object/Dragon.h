#ifndef DRAGON_H
#define DRAGON_H

#include "../XRObject.h"
#include "../shader/PhongLightingGS.h"

class Dragon :public XRObject
{
public:

	Dragon() :XRObject("Dragon") {}

	/*extended from XREntity*/
	virtual bool initObject();
	virtual bool updateObject(double time);
	virtual bool destroyObject();

};


#endif