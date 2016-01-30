#include <stddef.h>
#include "XREngine.h"
#include ".\scene\ViewerScene\ViewerScene.h"

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

XREngine::XREngine(){}

bool XREngine::init(GLFWwindow* window)
{
	//set window
	setGLFWwindow(window);

	//create application
	scene = new ViewerScene();
	scene->init();

	return true;
}

bool XREngine::update()
{
	scene->update(0);
	return true;
}

bool XREngine::destroy()
{
	scene->destroy();
	return true;
}
