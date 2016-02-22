#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include "../XRObject.h"
#include "../shader/PhongLightingGS.h"

class PointCloud :public XRObject
{
public:
	/*extended from XREntity*/
	virtual bool initObject();
	virtual bool updateObject(double time);
	virtual bool destroyObject();

private:
	int pointNum;
	Material material;
	PointLight light;
	GLuint uboMaterial;
	GLuint uboLight;


	glm::vec3 position;
	GLuint vao;
	GLuint program;

	bool render();
	
};




#endif