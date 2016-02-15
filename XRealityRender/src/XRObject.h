#ifndef XROBJECT_H
#define XROBJECT_H

#include "XREntity.h"
#include "XRCamera.h"
#include "glm\glm.hpp"

/**
* XRObject
* XRObject is a visible object in the scene
*
* @Author Yu Mao
*/
class XRObject:public XREntity 
{
public:
	/*extended from XREntity*/
	virtual bool init(){ return true; }
	virtual bool update(double time) = 0;
	virtual bool destroy(){ return true; }
	virtual bool render(){ return true; }
	inline void setCamera(XRCamera *camera){ this->camera = camera; }
public:
	glm::vec3 position;
	XRCamera *camera;
	GLuint vao;
	GLuint program;
};

#endif