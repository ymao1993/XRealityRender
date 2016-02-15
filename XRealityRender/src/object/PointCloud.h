#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include "../XRObject.h"

class PointCloud :public XRObject
{
public:
	/*extended from XREntity*/
	virtual bool init();
	virtual bool update(double time);
	virtual bool destroy();

	/*extended from XRObject*/
	virtual bool render();

private:
	int pointNum;
};




#endif