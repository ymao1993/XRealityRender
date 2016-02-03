#ifndef XROBJECT_H
#define XROBJECT_H

#include "XREntity.h"
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
public:
	glm::vec3 position;
	glm::mat4 transform;
	GLuint vao;
	GLuint program;
};

#endif