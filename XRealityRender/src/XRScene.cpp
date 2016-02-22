#include "XRScene.h"

#include <algorithm>

bool XRScene::init()
{
	//init ui
	ui = createUserInterface();
	ui->init();

	//init scene
	initScene();

	for (std::list<XRObject*>::iterator iter = objects.begin(); iter != objects.end(); iter++)
	{
		(*iter)->init();
	}

	return true;
}

bool XRScene::update(double deltaTime)
{
	updateScene(deltaTime);

	for (std::list<XRObject*>::iterator iter = objects.begin(); iter != objects.end(); iter++)
	{
		(*iter)->update(deltaTime);
	}

	ui->update(deltaTime);

	return true;
}

bool XRScene::destroy()
{
	destroyScene();
	ui->destroy();
	delete ui;

	for (std::list<XRObject*>::iterator iter = objects.begin(); iter != objects.end(); iter++)
	{
		(*iter)->destroy();
	}

	return true;
}


void XRScene::addObject(XRObject* object)
{
	if (object)
	{
		objects.push_back(object);
	}
}

void XRScene::deleteObject(XRObject* object)
{
	if (object && std::find(objects.begin(), objects.end(), object) != objects.end())
	{
		object->destroy();
		delete object;
		objects.push_back(object);
	}
}

void XRScene::deleteAllObjects()
{
	for (std::list<XRObject*>::iterator iter = objects.begin(); iter != objects.end(); iter++)
	{
		(*iter)->destroy();
		delete *iter;
	}
	objects.clear();
}