#ifndef MESHKDTREE_H
#define MESHKDTREE_H
#include <vector>
#include "glm\glm.hpp"

class XRMesh;

using namespace glm;

struct MeshKDTreeNode
{
	int splitAxis;
	float splitValue;
	MeshKDTreeNode* left;
	MeshKDTreeNode* right;
	MeshKDTreeNode* parent;
	std::vector<int> triangleIdices;

	MeshKDTreeNode() :splitAxis(0), splitValue(0), left(0), right(0){};
};

struct MeshTriangle
{
	vec3 v1, v2, v3, vc;
	vec3 n1, n2, n3, nc;

	MeshTriangle(vec3 v1, vec3 v2, vec3 v3, vec3 n1, vec3 n2, vec3 n3) :
		v1(v1), v2(v2), v3(v3),
		vc((v1 + v2 + v3) / 3.f),
		n1(n1), n2(n2), n3(n3),
		nc((n1 + n2 + n3) / 3.f){}
};

/**
* structure describing intersection
*/
struct MeshIntersect
{
	glm::vec3 position;
	glm::vec3 normal;
	MeshTriangle triangle;
};

class MeshKDTree
{
public:

	/**
	* constructor
	*/
	MeshKDTree(XRMesh* mesh) :mesh(mesh){}

	/**
	* construct MeshKDTree from data
	*/
	void construct();

	/**
	* check whether a moving point collides with the mesh
	*/
	bool pointCollide(glm::vec3 point, glm::vec3 point_pre, MeshIntersect& intersect);

	~MeshKDTree();

private:

	XRMesh *mesh;
	MeshKDTreeNode* root;
	std::vector<MeshTriangle> triangles;

	void constructNode(MeshKDTreeNode* node, int axis);
	void deleteNode(MeshKDTreeNode* node);

};


#endif