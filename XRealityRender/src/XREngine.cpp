#include "gl3w\gl3w.h"
#include "XREngine.h"
#include "XROVRCamera.h"
#include <stddef.h>
#include ".\scene\ViewerScene\ViewerScene.h"
#include <time.h>

#include <Windows.h>


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
	XRDevice::init();

	//create application
	scene = new ViewerScene();
	scene->init();

	ovrCamera = new XROVRCamera();
	ovrCamera->init();
	ovrCamera->setScene(scene);
	scene->setOVRCamera(ovrCamera);

	return true;
}

bool XREngine::update()
{
	//double begin = glfwGetTime();

	//update device status
	XRDevice::update(0);

	//handle key event
	//handleKeyEvent();

	//update scene
	//scene->update(deltaTime);  //TODO: sort of strange, maybe create a new class named gameMode to manage the scene?

	ovrCamera->update(0);

	//maintain fps
	//while ((deltaTime = glfwGetTime() - begin) < secondsPerFrame);

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
	ovrCamera->destroy();
	delete ovrCamera;

	scene->destroy();
	delete scene;

	return true;
}
