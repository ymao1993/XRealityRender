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

	bool readPts(std::vector<PointCloudPoint>& result, const char* filePath);
	GLfloat* readPts(const char* filePath, int &vertexCount);
	bool writeToFile(std::vector<PointCloudPoint> pointCloud);

}
#endif
