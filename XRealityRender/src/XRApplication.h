#ifndef XRAPPLICATION_H
#define XRAPPLICATION_H

#include <GLFW\glfw3.h>

#include "XREntity.h"
#include "RotatingTriangle.h"

class XRApplication:public XREntity
{
public:
	virtual bool init();
	virtual bool update(float time);
	virtual bool destroy();
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
private:
	RotatingTriangle *triangle;
};


#endif