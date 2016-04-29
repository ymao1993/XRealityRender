#ifndef XROBJLOADER_H
#define XROBJLOADER_H

namespace XRObjLoader
{
	bool loadObj(const char* filePath, int &vnum, int &fnum, float** vp, float** vn, float** vtx);
}

#endif