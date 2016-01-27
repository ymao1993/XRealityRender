#include "XRApplication.h"
#include "RotatingTriangle.h"
#include "GLFW\glfw3.h"


void XRApplication::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		printf("hello world!\n");
}


bool XRApplication::init()
{
	triangle = new RotatingTriangle();
	return true;
}

bool XRApplication::update(float time)
{
	triangle->update(time);
	return true;
}

bool XRApplication::destroy()
{
	delete triangle;
	return true;
}