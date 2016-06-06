#include "XRScene.h"

#include <algorithm>

bool XRScene::init()
{
	//init camera
	camera = new XRCamera();
	camera->init();

	//init ui
	gui = createUserInterface();
	gui->init();

	//init scene
	initScene();

	for (std::vector<XRObject*>::iterator iter = objects.begin(); iter != objects.end(); iter++)
	{
		(*iter)->init();
	}

	return true;
}

bool XRScene::update(double deltaTime)
{
	brush.Iclear();

	//update camera
	camera->update(deltaTime);

	updateScene(deltaTime);

	for (std::vector<XRObject*>::iterator iter = objects.begin(); iter != objects.end(); iter++)
	{
		(*iter)->update(deltaTime);
	}

	brush.render();

	gui->update(deltaTime);

	return true;
}

bool XRScene::destroy()
{
	destroyScene();
	gui->destroy();
	delete gui;

	for (std::vector<XRObject*>::iterator iter = objects.begin(); iter != objects.end(); iter++)
	{
		(*iter)->destroy();
	}

	return true;
}


void XRScene::addObject(XRObject* object)
{
	if (object)
	{
		object->scene = this;
		objects.push_back(object);
		if (object->name == "Skybox") //TODO: FIX it with a better design
			skybox = (XRSkybox*)object;
	}
}

void XRScene::deleteObject(XRObject* object)
{
	if (object)
	{
		auto iter = std::find(objects.begin(), objects.end(), object);
		if (iter != objects.end())
		{
			if ((*iter)->name == "Skybox") skybox == NULL;

			object->destroy();
			delete object;
			objects.erase(iter, iter + 1);
		}
	}
}

void XRScene::deleteAllObjects()
{
	for (std::vector<XRObject*>::iterator iter = objects.begin(); iter != objects.end(); iter++)
	{
		(*iter)->destroy();
		delete *iter;
	}
	objects.clear();
	skybox = NULL;
}

XRObject* XRScene::getObject(std::string name)
{
	for (std::vector<XRObject*>::iterator iter = objects.begin(); iter != objects.end(); iter++)
	{
		if ((*iter)->name == name)
		{
			return *iter;
		}
	}
	return NULL;
}