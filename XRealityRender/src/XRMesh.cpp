#include "XRMesh.h"

#include "XRDebug.h"
#include "utils\XRObjLoader.h"

bool XRMesh::loadMesh(const char* filePath)
{
	vertexNum = 0;
	int faceNum;
	XRDebug::log("start loading mesh...");
	bool result = XRObjLoader::loadObj(filePath, vertexNum, faceNum, &positions, &normals);
	if (result)
	{
		XRDebug::log("reading complete.");
		return true;
	}
	else
	{
		XRDebug::log("reading fails.");
		return false;
	}
}


bool XRMesh::destroy()
{
	free(positions);
	free(normals);
	return true;
}

