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

	/**
	 * Generate a cube mesh
	 */
	void generateCube(XRMesh& mesh, vec3 center, float xSpan, float ySpan, float zSpan);
}

#endif