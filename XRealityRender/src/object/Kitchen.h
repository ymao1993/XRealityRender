#ifndef KITCHEN
#define KITCHEN

#include "../XRObject.h"
#include "../shader/PhongLightingGS.h"

class Kitchen :public XRObject
{
public:
	/*extended from XREntity*/
	virtual bool initObject();
	virtual bool updateObject(double time);
	virtual bool destroyObject();

private:
	int pointNum;
	PointLight light;
	GLuint uboLight;

};





#endif