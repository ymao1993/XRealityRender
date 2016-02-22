#include "XRComponent.h"
#include "XRComponentType.h"

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
};

class XRMesh: public XRComponent
{
public:

	XRMesh() :XRComponent(XR_COMPONENT_MESH){}

	bool loadMesh(const char* filePath);

	float* positions;
	float* normals;
	int vertexNum;

	void setType(XRMESH_TYPE type){ this->type = type; }
	XRMESH_TYPE getType(){ return this->type; }
private:
	virtual bool init(){ return true; };
	virtual bool update(double time){ return true; }
	virtual bool destroy();

	XRMESH_TYPE type;

};