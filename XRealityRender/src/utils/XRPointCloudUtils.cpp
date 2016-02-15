#include "../XRCommon.h"
#include "XRPointCloudUtils.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace XRPointCloudUtils
{
	bool readPts(std::vector<PointCloudPoint>& result, const char* filePath)
	{
		std::ifstream infile;
		std::string line;

		infile.open(filePath);
		if (infile.is_open())
		{
			int i = 0;
			while (std::getline(infile, line))
			{
				// parse string space delimited
				//std::cout << line << std::endl;
				std::istringstream buf(line);
				std::istream_iterator<std::string> beg(buf), end;
				std::vector<std::string> tokens(beg, end); // done
				//for (auto& s : tokens)
				//	std::cout << '"' << s << '"' << '\n';
				i++;
				//std::cout << tokens.size() << std::endl;
				if (tokens.size() == 7) {
					PointCloudPoint newpoint;
					newpoint.x = std::stof(tokens[0]);
					newpoint.y = std::stof(tokens[1]);
					newpoint.z = std::stof(tokens[2]);
					newpoint.r = std::stoi(tokens[3]);
					newpoint.g = std::stoi(tokens[4]);
					newpoint.b = std::stoi(tokens[5]);
					newpoint.alpha = std::stoi(tokens[6]);
					result.push_back(newpoint);
				}
				//printf("%d\n", i);
			};
		}
		else
		{
			XRDebug::log("point cloud file does not exist.",XRDebug::XRDEBUG_LOGLEVEL_ERROR);
			return false;
		}
		return true;

	}

	GLfloat* readPts(const char* filePath, int &vertexCount)
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
					result = (GLfloat*)malloc(std::stoi(tokens[0])*sizeof(GLfloat)*4);
					continue;
				}

				//std::cout << tokens.size() << std::endl;
				if (tokens.size() == 5) {
					result[vertexCount * 4 + 0] = std::stof(tokens[0]);
					result[vertexCount * 4 + 1] = std::stof(tokens[1]);
					result[vertexCount * 4 + 2] = std::stof(tokens[2]);
					result[vertexCount * 4 + 3] = 1;
					vertexCount++;
				}
			};
			vertexCount = vertexCount;
		}
		else
		{
			XRDebug::log("point cloud file does not exist.", XRDebug::XRDEBUG_LOGLEVEL_ERROR);
		}
		return result;
	}

	bool writeToFile(std::vector<PointCloudPoint> pointCloud)
	{
		// not done
		return true;
	}




};
