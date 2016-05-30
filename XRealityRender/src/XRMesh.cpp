#include "XRMesh.h"
#include "XRDebug.h"
#include "utils\XRObjLoader.h"
#include "XRObject.h"
#include "XRScene.h"

bool XRMesh::loadMesh(const char* filePath)
{
	vertexNum = 0;
	faceNum = 0;
	idxNum = 0;
	XRDebug::log("start loading mesh...");
	bool result;
	if (type == XRMESH_TRIANGLE_SOUP)
	{
		result = XRObjLoader::loadObj(filePath, vertexNum, faceNum, idxNum, &positions, &normals, &txcoords);
	}
	else if(type == XRMESH_TRIANGLE_SOUP_INDEXED)
	{
		result = XRObjLoader::loadObj(filePath, vertexNum, faceNum, idxNum, &positions, &normals, &txcoords, &indices);
	}
	else
	{
		XRDebug::logE("mesh type not supported by the loader.");
	}
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

void XRMesh::computeBbox()
{
	for (int i = 0; i < vertexNum; i++)
	{
		bbox.expand(glm::vec3(positions[3 * i], positions[3 * i + 1], positions[3 * i + 2]));
	}

	// draw bounding box
	{
		vec4 worldCorner_min = object->model2World * vec4(bbox.corner_min, 1);
		vec4 worldCorner_max = object->model2World * vec4(bbox.corner_max, 1);
		worldCorner_min /= worldCorner_min.w;
		worldCorner_max /= worldCorner_min.w;

		double worldLx = (worldCorner_max - worldCorner_min).x;
		double worldLy = (worldCorner_max - worldCorner_min).y;
		double worldLz = (worldCorner_max - worldCorner_min).z;

		object->scene->brush.drawBox(vec3(worldCorner_min), worldLx, worldLy, worldLz, false);
	}
}

void XRMesh::computeHeightField()
{
	heightField.init(this);
	vec4 worldCorner_min = object->model2World * vec4(bbox.corner_min, 1);
	worldCorner_min /= worldCorner_min.w;

	// //draw height field
	//{
	//	for (int j = 0; j < heightField.numCols; j++)
	//	{
	//		for (int i = 0; i < heightField.numRows; i++)
	//		{
	//			vec4 worldCell = object->model2World * vec4(heightField.minX + i * heightField.cellSize, bbox.corner_min.y, heightField.minZ + j * heightField.cellSize, 1);
	//			vec4 worldCellWHD = object->model2World * vec4(heightField.cellSize, heightField.height[i + j * heightField.numRows] - bbox.corner_min.y, heightField.cellSize, 0);
	//			worldCell /= worldCell.w;

	//			object->scene->brush.drawBox(vec3(worldCell), worldCellWHD.x, worldCellWHD.y, worldCellWHD.z, false);

	//		}
	//	}
	//}
	//// draw normal
	//{
	//	for (int j = 0; j < heightField.numCols; j++)
	//	{
	//		for (int i = 0; i < heightField.numRows; i++)
	//		{	
	//			vec4 worldCell = object->model2World * vec4(heightField.minX + i * heightField.cellSize, heightField.height[i + j * heightField.numRows], heightField.minZ + j * heightField.cellSize, 1);
	//			worldCell /= worldCell.w;
	//			vec3 normal = glm::normalize(heightField.normal[i + j * heightField.numRows]);
	//			vec4 tmp = object->model2World * vec4(normal.x, normal.y, normal.z, 0);
	//			object->scene->brush.drawLine(vec3(worldCell), vec3(worldCell) + normal * 0.01f, false);
	//			
	//		}
	//	}
	//}
}


bool XRMesh::destroy()
{
	free(positions);
	free(normals);
	return true;
}

