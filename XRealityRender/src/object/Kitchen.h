#ifndef KITCHEN
#define KITCHEN

#include "../XRObject.h"
#include "../shader/PhongLightingGS.h"

class Kitchen :public XRObject
{
public:

	Kitchen() :XRObject("Kitchen"){}

	/*extended from XREntity*/
	virtual bool initObject();
	virtual bool updateObject(double time);
	virtual bool destroyObject();

private:
	int pointNum;

};





#endif