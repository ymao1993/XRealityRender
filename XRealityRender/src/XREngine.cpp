#include "XREngine.h"
#include <stddef.h>
#include <time.h>
#include <Windows.h>
#include "XRShaderManager.h"
#include ".\scene\ForestScene\ForestScene.h"


XREngine* XREngine::engine = NULL;

XREngine* XREngine::instance()
{
	if (engine)
	{
		return engine;
	}
	else
	{
		engine = new XREngine();
		return engine;
	}
}

XREngine::XREngine():
deltaTime(1./60),
secondsPerFrame(1./60)
{}

bool XREngine::init(GLFWwindow* window)
{
	//set window
	setGLFWwindow(window);

	//initialize device status
	XRDevice::init(window);

	//initialize shader manager
	XRShaderManger::init(/*true*/);

	//create application
	scene = new ForestScene();
	scene->init();

	return true;
}

bool XREngine::update()
{
	double begin = glfwGetTime();

	//update device status
	XRDevice::update(deltaTime);

	//handle key event
	handleKeyEvent();

	//update scene
	scene->update(deltaTime);  //TODO: sort of strange, maybe create a new class named gameMode to manage the scene?

	//maintain fps
	while ((deltaTime = glfwGetTime() - begin) < secondsPerFrame);

	return true;
}

void XREngine::handleKeyEvent()
{
	//exit when the user hit esc
	if (XRDevice::isKeyPressed(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
	}
}

bool XREngine::destroy()
{
	XRShaderManger::free();
	scene->destroy();
	return true;
}
