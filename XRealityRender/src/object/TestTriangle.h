#ifndef TESTTRIANGLE_H
#define TESTTRIANGLE_H

#include "../XRObject.h"

class TestTriangle :public XRObject
{
public:
	/*extended from XREntity*/
	virtual bool initObject();
	virtual bool updateObject(double time);
	virtual bool destroyObject();

	/*extended from XRObject*/
	virtual bool render();

private:

	glm::vec3 position;
	GLuint vao;
	GLuint program;
};


#endif