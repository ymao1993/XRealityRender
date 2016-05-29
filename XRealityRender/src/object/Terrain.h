#ifndef TERRAIN_H
#define TERRAIN_H

#include "../XRCommon.h"
#include "../XRObject.h"

class Terrain :public XRObject
{
public:

	Terrain(vec3 center, vec2 numCell, float cellSize = 1.f):
	XRObject("Terrain"),
	center(center),
	numCell(numCell),
	cellSize(cellSize)
	{}

	/*extended from XREntity*/
	virtual bool initObject();
	virtual bool updateObject(double time);
	virtual bool destroyObject();

private:

	vec2 numCell;
	vec3 center;
	float cellSize;

};


#endif
