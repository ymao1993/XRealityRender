#ifndef BOXFLUID
#define BOXFLUID

#include "../XRObject.h"
#include "../shader/PhongLightingGS.h"
#include "../simulation/SPHfluid/SPHSimulator.h"

class BoxFluid :public XRObject
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

	SPHSim::SPHSimulator simulator;

};





#endif