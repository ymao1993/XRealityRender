#include <GLFW\glfw3.h>
#include "RotatingTriangle.h"



bool RotatingTriangle::init()
{
	return true;
}

bool RotatingTriangle::update(double time)
{
	float ratio;
	int width = XREngine::instance()->getWindowW();
	int height = XREngine::instance()->getWindowH();
	ratio = width / (float)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-0.6f, -0.4f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.6f, -0.4f, 0.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.6f, 0.f);
	glEnd();

	return true;
}

bool RotatingTriangle::destroy()
{
	return true;
}