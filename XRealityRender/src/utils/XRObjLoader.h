#ifndef XROBJLOADER_H
#define XROBJLOADER_H

namespace XRObjLoader
{
	/**
	 * load obj file
	 * when indices is NULL, this will return non-indexed vertex attributes
	 * else this will return indexed vertex attributes
	 */
	bool loadObj(const char* filePath, int &vnum, int &fnum, int &inum, float** vp, float** vn, float** txc, int** indices= NULL);
}

#endif