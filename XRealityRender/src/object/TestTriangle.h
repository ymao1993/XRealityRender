#ifndef TESTTRIANGLE_H
#define TESTTRIANGLE_H

#include "../XRObject.h"

class TestTriangle :public XRObject
{
public:
	/*extended from XREntity*/
	virtual bool init();
	virtual bool update(double time);
	virtual bool destroy();

	/*extended from XRObject*/
	virtual bool render();
};




#endif