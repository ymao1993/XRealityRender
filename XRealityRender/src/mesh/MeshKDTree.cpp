#include "MeshKDTree.h"
#include "../XRMesh.h"

#include <algorithm>

void MeshKDTree::constructNode(MeshKDTreeNode* node, int axis)
{
	if (node)
	{
		//TODO: determine split value
		const std::vector<int>& indices = node->triangleIdices;

		std::vector<float> centers;
		for (int i = 0; i < indices.size(); i++)
		{
			centers.push_back(triangles[indices[i]].vc[axis]);
		}
		std::sort(centers.begin(), centers.end());
		node->splitValue = centers[centers.size() / 2];
		node->splitAxis = axis;
		node->left = new MeshKDTreeNode();
		node->right = new MeshKDTreeNode();
		node->left->parent = node;
		node->right->parent = node;

		for (int i = 0; i <indices.size(); i++)
		{
			if (triangles[indices[i]].v1[axis] <= node->splitValue ||
				triangles[indices[i]].v2[axis] <= node->splitValue ||
				triangles[indices[i]].v3[axis] <= node->splitValue)
			{
				node->left->triangleIdices.push_back(indices[i]);
			}

			if (triangles[indices[i]].v1[axis] >= node->splitValue ||
				triangles[indices[i]].v2[axis] >= node->splitValue ||
				triangles[indices[i]].v3[axis] >= node->splitValue)
			{
				node->right->triangleIdices.push_back(indices[i]);
			}
		}

		//if the split cannot make the node's triangle number smaller than its parrent
		//then we should not split
		if (node->left->triangleIdices.size() == node->triangleIdices.size() || 
			node->right->triangleIdices.size() == node->triangleIdices.size())
		{
			delete node->left;
			node->left = NULL;
			delete node->right;
			node->right = NULL;
			return;
		}
		constructNode(node->left, (axis + 1) % 3);
		constructNode(node->right, (axis + 1) % 3);
	}
	return;
}

void MeshKDTree::construct()
{
	//feed in triangle data
	for (int i = 0; i < mesh->vertexNum; i += 3)
	{
		vec3 v1(mesh->positions[3 * i + 0], mesh->positions[3 * i + 1], mesh->positions[3 * i + 2]);
		vec3 v2(mesh->positions[3 * i + 3], mesh->positions[3 * i + 4], mesh->positions[3 * i + 5]);
		vec3 v3(mesh->positions[3 * i + 6], mesh->positions[3 * i + 7], mesh->positions[3 * i + 8]);
		vec3 n1(mesh->normals[3 * i + 0], mesh->normals[3 * i + 1], mesh->normals[3 * i + 2]);
		vec3 n2(mesh->normals[3 * i + 3], mesh->normals[3 * i + 4], mesh->normals[3 * i + 5]);
		vec3 n3(mesh->normals[3 * i + 6], mesh->normals[3 * i + 7], mesh->normals[3 * i + 8]);

		triangles.push_back(MeshTriangle(v1, v2, v3, n1, n2, n3));
	}

	//construct node
	std::vector<int> rootTrigs(triangles.size());
	for (int i = 0; i < rootTrigs.size(); i++)
	{
		rootTrigs[i] = i;
	}
	root = new MeshKDTreeNode;
	root->triangleIdices = rootTrigs;
	root->parent = NULL;
	constructNode(root, 0);
}

bool searchNode()
{
	return true;
}

bool MeshKDTree::pointCollide(glm::vec3 point, glm::vec3 point_pre, MeshIntersect& intersect)
{
	int start_axis = 0;
	return true;
}

void MeshKDTree::deleteNode(MeshKDTreeNode* node)
{
	if (node)
	{
		deleteNode(node->left);
		deleteNode(node->right);
		delete node;
	}
	return;
}

MeshKDTree::~MeshKDTree()
{
	deleteNode(root);
}