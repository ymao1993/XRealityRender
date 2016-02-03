#include "..\..\gl3w\gl3w.h"
#include "ViewerScene.h"
#include "ViewerSceneGUI.h"
#include "../../object/TestTriangle.h"
#include "../../object/PointCloud.h"



bool ViewerScene::initScene()
{
	//init camera
	camera = new XRCamera();
	camera->init();

	//init game object
	//testObj = new TestTriangle();
	testObj = new PointCloud();

	testObj->init();

	return true;
}

XRUserInterface* ViewerScene::createUserInterface()
{
	return new ViewerSceneGUI(this);
}

bool ViewerScene::updateScene(double time)
{
	//update camera
	camera->update(time);

	//testObj->update(time);
	testObj->transform = camera->getWorld2Clip();
	testObj->update(time);
	testObj->render();

	return true;
}

bool ViewerScene::destroyScene()
{
	//destroy camera
	camera->destroy();
	delete camera;

	//destroy object
	testObj->destroy();
	delete testObj;

	return true;
}