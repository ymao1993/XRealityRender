#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include <tinyobjloader\tiny_obj_loader.h>

#include "../XRDebug.h"


namespace XRObjLoader
{
	bool loadObj(const char* filePath, int &vnum, int &fnum, int &inum, float** vp, float** vn, float** txc, int** indices)
	{
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err;
		bool ret = tinyobj::LoadObj(shapes, materials, err, filePath);

		if (!err.empty()) { // `err` may contain warning message.
			XRDebug::logW(err.c_str());
		}

		if (!ret) {
			return false;
		}

		if (indices == NULL) //loaded as non-indexed format
		{
			vnum = 0;
			for (int i = 0; i < shapes.size(); i++)
				vnum += shapes[i].mesh.indices.size();
			fnum = vnum / 3;

			bool containNormal = shapes[0].mesh.normals.size() != 0;
			bool containTxc = shapes[0].mesh.texcoords.size() != 0;
			*vp = (float*)malloc(sizeof(float)* vnum * 3);
			*vn = containNormal ? (float*)malloc(sizeof(float)* vnum * 3) : 0;
			*txc = containTxc ? (float*)malloc(sizeof(float)* vnum * 2) : 0;
			int vpCount = 0;
			int vnCount = 0;
			int txcCount = 0;
			for (size_t j = 0; j < shapes.size(); j++)
			{
				for (size_t i = 0; i < shapes[j].mesh.indices.size(); i++) {
					int idx = shapes[j].mesh.indices[i];
					(*vp)[vpCount++] = shapes[j].mesh.positions[idx * 3 + 0];
					(*vp)[vpCount++] = shapes[j].mesh.positions[idx * 3 + 1];
					(*vp)[vpCount++] = shapes[j].mesh.positions[idx * 3 + 2];
					if (containNormal)
					{
						(*vn)[vnCount++] = shapes[j].mesh.normals[idx * 3 + 0];
						(*vn)[vnCount++] = shapes[j].mesh.normals[idx * 3 + 1];
						(*vn)[vnCount++] = shapes[j].mesh.normals[idx * 3 + 2];
					}
					if (containTxc)
					{
						(*txc)[txcCount++] = shapes[j].mesh.texcoords[idx * 2 + 0];
						(*txc)[txcCount++] = shapes[j].mesh.texcoords[idx * 2 + 1];
					}
				}
			}
		}
		else //loaded as indexed format
		{
			vnum = fnum = 0;
			for (int i = 0; i < shapes.size(); i++)
			{
				vnum += shapes[i].mesh.positions.size() / 3;
				fnum += shapes[i].mesh.indices.size() / 3;
			}
			inum = fnum * 3;

			bool containNormal = shapes[0].mesh.normals.size() != 0;
			bool containTxc = shapes[0].mesh.texcoords.size() != 0;
			*vp = (float*)malloc(sizeof(float)* vnum * 3);
			*vn = containNormal ? (float*)malloc(sizeof(float)* vnum * 3) : 0;
			*txc = containTxc ? (float*)malloc(sizeof(float)* vnum * 2) : 0;
			*indices = (int*)malloc(sizeof(int)* fnum * 3);
			int vpCount = 0;
			int vnCount = 0;
			int txcCount = 0;
			int indicesCount = 0;

			int indicesBase = 0;
			for (size_t j = 0; j < shapes.size(); j++)
			{
				for (size_t i = 0; i < shapes[j].mesh.indices.size(); i++) 
				{
					(*indices)[indicesCount++] = indicesBase + (int)(shapes[j].mesh.indices[i]);
				}
				indicesBase += shapes[j].mesh.positions.size()/3;

				memcpy(*vp + vpCount, &shapes[j].mesh.positions[0], sizeof(float)* shapes[j].mesh.positions.size());
				vpCount += shapes[j].mesh.positions.size();

				if (containNormal)
				{
					memcpy(*vn + vnCount, &shapes[j].mesh.normals[0], sizeof(float)* shapes[j].mesh.normals.size());
					vnCount += shapes[j].mesh.normals.size();
				}

				if (containTxc)
				{
					memcpy(*txc + txcCount, &shapes[j].mesh.texcoords[0], sizeof(float)* shapes[j].mesh.texcoords.size());
					txcCount += shapes[j].mesh.texcoords.size();
				}
			}
		}

		return true;
	}
}