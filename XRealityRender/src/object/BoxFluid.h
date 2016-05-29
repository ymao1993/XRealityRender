#ifndef BOXFLUID
#define BOXFLUID

#include "../XRObject.h"
#include "../shader/PhongLightingGS.h"
#include "../simulation/SPHfluid/SPHSimulator.h"

class BoxFluid :public XRObject
{
public:

	BoxFluid() :XRObject("Box Fluid"){}

	/*extended from XREntity*/
	virtual bool initObject();
	virtual bool updateObject(double time);
	virtual bool destroyObject();

private:
	PointLight light;
	GLuint uboLight;

	SPHSim::SPHSimulator *simulator;

};





#endif