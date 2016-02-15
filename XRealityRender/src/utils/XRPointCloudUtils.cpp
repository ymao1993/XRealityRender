#include "../XRCommon.h"
#include "XRPointCloudUtils.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace XRPointCloudUtils
{
	void readPts(const char* filePath, int &vertexCount, float** positions, float** normals)
	{
		GLfloat* result = NULL;
		std::ifstream infile;
		std::string line;

		infile.open(filePath);

		if (infile.is_open())
		{
			int i = 0;
			vertexCount = 0;
			while (std::getline(infile, line))
			{
				// parse string space delimited
				std::istringstream buf(line);
				std::istream_iterator<std::string> beg(buf), end;
				std::vector<std::string> tokens(beg, end); // done
				i++;

				if (i == 1)
				{
					*positions = (GLfloat*)malloc(std::stoi(tokens[0])*sizeof(GLfloat)* 4);
					*normals   = (GLfloat*)malloc(std::stoi(tokens[0])*sizeof(GLfloat)* 3);
					continue;
				}

				//std::cout << tokens.size() << std::endl;
				if (tokens.size() >= 6) {
					(*positions)[vertexCount * 4 + 0] = std::stof(tokens[0]);
					(*positions)[vertexCount * 4 + 1] = std::stof(tokens[1]);
					(*positions)[vertexCount * 4 + 2] = std::stof(tokens[2]);
					(*positions)[vertexCount * 4 + 3] = 1;
					(*normals)[vertexCount * 3 + 0] = std::stof(tokens[3]);
					(*normals)[vertexCount * 3 + 1] = std::stof(tokens[4]);
					(*normals)[vertexCount * 3 + 2] = std::stof(tokens[5]);
					vertexCount++;
				}
			};
		}
		else
		{
			XRDebug::log("point cloud file does not exist.", XRDebug::XRDEBUG_LOGLEVEL_ERROR);
		}
		return;
	}

	bool writeToFile(std::vector<PointCloudPoint> pointCloud)
	{
		// not done
		return true;
	}

};
