#ifndef HEIGHT_FIELD_H
#define HEIGHT_FIELD_H
#include <stdio.h>
#include <glm/glm.hpp>

class XRMesh;


struct HeightField
{
public:
	HeightField(double cellSize) :cellSize(cellSize), height(0), normal(0) {}
	~HeightField(){};
	void reset();

	/**
	 * init height field from a mesh
	 */
	void init(const XRMesh *mesh);

	/**
	 * collisionDetection
	 * detect collision of query point, return estimated normal of collision
	 * @param pos position of the query point
	 * @param normal estimated normal of collision
	 * @nearestSurfPos nearest surface point to the position
	 * @ret true if collision happens, else false
	 */
	bool collisionDetection(const glm::vec3& pos, glm::vec3& normal, glm::vec3& nearestSurfPos);

private:

	/**
	* compute the cell index corresponding to the query points' position
	* @param pos position of the query point
	* @param cell cell's x,z index
	* @ret true if the query point is inside the height field, false if not.
	*/
	inline bool queryCell(glm::vec3 pos, glm::vec2& cell);

	/**
	 * get height
	 */
	inline double getHeight(const glm::vec2& cell);

public:
	double cellSize;
	glm::vec3 position;
	double* height;
	glm::vec3* normal;

	int numRows;
	int numCols;
	float minX;
	float minZ;
	float maxX;
	float maxZ;
};

#endif