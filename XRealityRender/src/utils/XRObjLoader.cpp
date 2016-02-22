#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include <tinyobjloader\tiny_obj_loader.h>

#include "../XRDebug.h"


namespace XRObjLoader
{
	bool loadObj(const char* filePath, int &vnum, int &fnum, float** vp, float** vn)
	{
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::string err;
		bool ret = tinyobj::LoadObj(shapes, materials, err, filePath);

		if (!err.empty()) { // `err` may contain warning message.
			XRDebug::log(err);
		}

		if (!ret) {
			return false;
		}

		assert(shapes.size() == 1);
		assert((shapes[0].mesh.indices.size() % 3) == 0);
		vnum = shapes[0].mesh.indices.size();
		fnum = shapes[0].mesh.indices.size() / 3;
		*vp = (float*)malloc(sizeof(float) * vnum * 3);
		*vn = (float*)malloc(sizeof(float)* vnum * 3);
		for (size_t i = 0; i < shapes[0].mesh.indices.size(); i++) {
			int idx = shapes[0].mesh.indices[i];
			(*vp)[i * 3 + 0] = shapes[0].mesh.positions[idx * 3 + 0];
			(*vp)[i * 3 + 1] = shapes[0].mesh.positions[idx * 3 + 1];
			(*vp)[i * 3 + 2] = shapes[0].mesh.positions[idx * 3 + 2];
			(*vn)[i * 3 + 0] = shapes[0].mesh.normals[idx * 3 + 0];
			(*vn)[i * 3 + 1] = shapes[0].mesh.normals[idx * 3 + 1];
			(*vn)[i * 3 + 2] = shapes[0].mesh.normals[idx * 3 + 2];
		}

		return true;
	}
}