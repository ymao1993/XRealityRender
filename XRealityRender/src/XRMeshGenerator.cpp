#include "XRMeshGenerator.h"


#define HORINZONTALPLANE_SOUP
//#define HORINZONTALPLANE_STRIP

namespace XRMeshGenerator
{
	void generateHorizontalPlane(XRMesh& mesh,
		vec3 center, int numCellX, int numCellZ, float cellSize)
	{

#if defined(HORINZONTALPLANE_STRIP)
		mesh.setType(XRMESH_TRIANGLE_STRIP_INDEXED);
#endif
#if defined(HORINZONTALPLANE_SOUP)
		mesh.setType(XRMESH_TRIANGLE_SOUP_INDEXED);
#endif

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


#if defined(HORINZONTALPLANE_STRIP)
		mesh.indices = (int*)malloc(sizeof(GLint) * (mesh.faceNum + numCellX + 1));
		mesh.indices[0] = 0;
		mesh.indices[1] = numVerticeZ;

		int count = 2;
		for (int j = 0; j < numCellX; j++)
		{
			int starti; int endi; int stepi;
			if (j % 2 == 0)
			{
				starti = 0; endi = numCellZ; stepi = 1;
			}
			else
			{
				starti = numCellZ; endi = 0; stepi = -1;
			}

			for (int i = starti; i != endi; i += stepi)
			{
				mesh.indices[count++] = j * numVerticeX + (i + stepi);
				mesh.indices[count++] = (j + 1) * numVerticeX + (i + stepi);
			}

			if (j != numCellX - 1)
			{
				mesh.indices[count++] = (j + 2) * numVerticeX + endi;
			}
		}
		mesh.idxNum = count;
#endif
#if defined(HORINZONTALPLANE_SOUP)
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
		mesh.idxNum = mesh.faceNum * 3;
#endif

	}
}