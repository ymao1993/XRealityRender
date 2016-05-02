#include "HeightField.h"
#include <iostream>
#include "..\XRMesh.h"

void HeightField::init(const XRMesh *mesh)
{
	// allocate grid by x-z boundary of bound box
	minX = (float)mesh->bbox.corner_min.x;
	minZ = (float)mesh->bbox.corner_min.z;
	maxX = (float)mesh->bbox.corner_max.x;
	maxZ = (float)mesh->bbox.corner_max.z;

	numRows = int(ceil((maxX - minX) / cellSize));
	numCols = int(ceil((maxZ - minZ) / cellSize));

	height = new double[numRows * numCols];
	normal = new vec3[numRows * numCols];
	for (int i = 0; i < numRows * numCols; i++)
	{
		height[i] = -INF_D;
	}

	// compute height field 
	for (int k = 0; k < mesh->vertexNum / 3; k++)
	{
		// raterization
		vec2 triBoundmin(min(min(mesh->positions[9 * k], mesh->positions[9 * k + 3]), mesh->positions[9 * k + 6]),		// x
						min(min(mesh->positions[9 * k + 2], mesh->positions[9 * k + 5]), mesh->positions[9 * k + 8]));	// z
		vec2 triBoundmax(max(max(mesh->positions[9 * k], mesh->positions[9 * k + 3]), mesh->positions[9 * k + 6]),		// x
						max(max(mesh->positions[9 * k + 2], mesh->positions[9 * k + 5]), mesh->positions[9 * k + 8]));	// z
		vec2 cell; // (x, z)

		for (int j = -1; j < ceil(triBoundmax.y - triBoundmin.y) / cellSize + 1; j++)
		{
			for (int i = -1; i < ceil(triBoundmax.x - triBoundmin.x) / cellSize + 1; i++)
			{
				// check if p is inside the triangle
				vec2 quantizedCell;
				queryCell(vec3(triBoundmin.x + i * cellSize, 0, triBoundmin.y + j * cellSize), quantizedCell);

				vec2 p(minX + quantizedCell.x * cellSize, minZ + quantizedCell.y * cellSize);
				vec2 v0(mesh->positions[9 * k + 6] - mesh->positions[9 * k], mesh->positions[9 * k + 8] - mesh->positions[9 * k + 2]);
				vec2 v1(mesh->positions[9 * k + 3] - mesh->positions[9 * k], mesh->positions[9 * k + 5] - mesh->positions[9 * k + 2]);
				vec2 v2(p - vec2(mesh->positions[9 * k], mesh->positions[9 * k + 2]));

				double dot00 = dot(v0, v0);
				double dot01 = dot(v0, v1);
				double dot02 = dot(v0, v2);
				double dot11 = dot(v1, v1);
				double dot12 = dot(v1, v2);

				double invDenorm = 1 / (dot00 * dot11 - dot01 * dot01);
				double u = (dot11 * dot02 - dot01 * dot12) * invDenorm;
				double v = (dot00 * dot12 - dot01 * dot02) * invDenorm;

				if ((u >= 0) && (v > 0 ) && (u + v) < 1)
				{

					if (queryCell(vec3(triBoundmin.x + i * cellSize, 0, triBoundmin.y + j * cellSize), cell))
					{
						// interpolation assign height
						double h = mesh->positions[9 * k + 1] + v * (mesh->positions[9 * k + 4] - mesh->positions[9 * k + 1]) + u * (mesh->positions[9 * k + 7] - mesh->positions[9 * k + 1]);
						double avg = (mesh->positions[9 * k + 1] + mesh->positions[9 * k + 4] + mesh->positions[9 * k + 7]) / 3;
						if (height[(int)cell.x + (int)cell.y * numRows] < h) // y-axis : height
						{
							height[(int)cell.x + (int)cell.y * numRows] = h;
						}
					}
				}

			}
		}
	}
	//reset 
	for (int i = 0; i < numRows * numCols; i++)
	{
		normal[i] = vec3(0, 0, 1);
	}
	// compute normal
	for (int j = 0; j < numCols - 1; j++)
	{
		for (int i = 0; i < numRows - 1; i++)
		{
			vec3 v1(cellSize, height[(i + 1) + j * numRows] - height[i + j * numRows], 0);
			vec3 v2(0, height[i + (j + 1) * numRows] - height[i + j * numRows], cellSize);

			if (!(cross(v2, v1) != cross(v2, v1)))
			{
				normal[i + j * numRows] = cross(v2, v1);
			}
			else
			{
				normal[i + j * numRows] = vec3(0, 1, 0);
			}
		}
	}
	
}
inline bool HeightField::queryCell(vec3 pos, vec2& cell)
{
	if (pos.x >= minX && pos.z >= minZ && pos.x <= maxX && pos.z <= maxZ)
	{
		cell = vec2(floor((pos.x - minX) / cellSize), floor((pos.z - minZ) / cellSize));
		return true;
	}
	else
	{
		return false;
	}
}

bool HeightField::collisionDetection(const glm::vec3& pos, glm::vec3& n, glm::vec3& nearestSurfPos)
{
	vec2 cell;		
	if (queryCell(pos, cell))
	{
		if (pos.y < height[(int)cell.x + numRows * (int)cell.y])
		{
			n = normal[(int)cell.x + numRows * (int)cell.y];
			nearestSurfPos = glm::vec3(minX + (int)cell.x * cellSize, height[(int)cell.x + numRows * (int)cell.y], minZ + (int)cell.y * cellSize);
			return true;
		}
		return false;
	}
	return false;
	
	
}