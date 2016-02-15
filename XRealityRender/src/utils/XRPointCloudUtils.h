#ifndef XRPOINTCLOUD_H
#define XRPOINTCLOUD_H

#include <vector>

namespace XRPointCloudUtils
{
	struct PointCloudPoint
	{
		float x, y, z, w;
		float r, g, b, alpha;
	};

	void readPts(const char* filePath, int &vertexCount, float** positions, float** normals);
	bool writeToFile(std::vector<PointCloudPoint> pointCloud);

}
#endif
