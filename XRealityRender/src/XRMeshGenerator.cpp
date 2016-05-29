#include "XRMeshGenerator.h"


namespace XRMeshGenerator
{
	void generateHorizontalPlane(XRMesh& mesh,
		vec3 center, int numCellX, int numCellZ, float cellSize)
	{
		mesh.setType(XRMESH_TRIANGLE_SOUP_INDEXED);
		mesh.vertexNum = (numCellX + 1) * (numCellZ + 1);
		mesh.faceNum = numCellX * numCellZ * 2;

		float xstart = center.x - cellSize * numCellX / 2;
		float zstart = center.z - cellSize * numCellZ / 2;

		//generate vertex position array and texture coordinates array
		mesh.positions = (float*)malloc(sizeof(float) * 3 * mesh.vertexNum);
		mesh.txcoords = (float*)malloc(sizeof(float) * 2 * mesh.vertexNum);
		int numVerticeX = numCellX + 1;
		int numVerticeZ = numCellZ + 1;
		for (int i = 0; i < numVerticeX; i++)
		{
			float x = xstart + i * cellSize;
			for (int j = 0; j < numVerticeZ; j++)
			{
				float z = zstart + j * cellSize;
				mesh.positions[3 * (numVerticeZ * i + j) + 0] = x;
				mesh.positions[3 * (numVerticeZ * i + j) + 1] = center.y;
				mesh.positions[3 * (numVerticeZ * i + j) + 2] = z;

				mesh.txcoords[2 * (numVerticeZ * i + j) + 0] = ((float)i)/numVerticeX;
				mesh.txcoords[2 * (numVerticeZ * i + j) + 1] = ((float)j)/numVerticeZ;
			}
		}

		//generate triangle index array
		mesh.indices = (int*)malloc(sizeof(int) * 3 * mesh.faceNum);
		for (int i = 0; i < numCellX; i++)
		{
			for (int j = 0; j < numCellZ; j++)
			{
				int cellIdx = numCellZ * i + j;
				
				int idx0 = numVerticeZ * i + j;
				int idx1 = numVerticeZ * (i + 1) + j;
				int idx2 = numVerticeZ * i + (j + 1);
				int idx3 = numVerticeZ * (i + 1) + (j + 1);

				mesh.indices[6 * cellIdx + 0] = idx0;
				mesh.indices[6 * cellIdx + 1] = idx2;
				mesh.indices[6 * cellIdx + 2] = idx1;
				mesh.indices[6 * cellIdx + 3] = idx1;
				mesh.indices[6 * cellIdx + 4] = idx2;
				mesh.indices[6 * cellIdx + 5] = idx3;
			}
		}



	}
}