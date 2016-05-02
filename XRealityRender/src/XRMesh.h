#ifndef XRMESH_H
#define XRMESH_H
#include "XRComponent.h"
#include "XRComponentType.h"
#include "mesh\MeshKDTree.h"
#include "geometry\AABBox.h"
#include "geometry\HeightField.h"

/**
* XRMesh represents the model of the object
*
* Currently XRMesh only supports polygon 
* soup representation.
*
* @Author Yu Mao
*/

enum XRMESH_TYPE
{
	XRMESH_TRIANGLE_SOUP,
	XRMESH_TRIANGLE_SOUP_INDEXED,
};

class XRMesh: public XRComponent
{
public:

	XRMesh(XRMESH_TYPE type = XRMESH_TRIANGLE_SOUP_INDEXED) : kdtree(this), heightField(0.5f), XRComponent(XR_COMPONENT_MESH), type(type){}
	XRMesh(float* positions, float* normals, float* txcoords, int vertexNum) :
		kdtree(this), heightField(0.5f),
		positions(positions), normals(normals), txcoords(txcoords), vertexNum(vertexNum), XRComponent(XR_COMPONENT_MESH){}

	bool loadMesh(const char* filePath);

	float* positions;
	float* normals;
	float* txcoords;
	int*   indices;

	int vertexNum;
	int faceNum;

	BBox bbox;
	HeightField heightField;
	MeshKDTree kdtree;

	void setType(XRMESH_TYPE type){ this->type = type; }
	XRMESH_TYPE getType(){ return this->type; }

	void computeBbox();
	void computeHeightField();

private:
	virtual bool init(){ return true; };
	virtual bool update(double time){ return true; }
	virtual bool destroy();

	XRMESH_TYPE type;

};

#endif