#ifndef XRMESHGENERATOR_H
#define XRMESHGENERATOR_H

#include "XRCommon.h"
#include "XRMesh.h"

namespace XRMeshGenerator
{
	/** 
	 * Generate A Hotinzontal Plane Mesh 
	 */
	void generateHorizontalPlane(XRMesh& mesh,
		vec3 center, int numCellX, int numCellZ, float cellSize);
}

#endif