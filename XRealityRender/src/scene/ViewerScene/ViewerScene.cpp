#include "..\..\gl3w\gl3w.h"
#include <gl\GL.h>
#include "ViewerScene.h"
#include "ViewerSceneGUI.h"
#include "../../object/TestTriangle.h"
#include "../../object/PointCloud.h"
#include "../../object/Kitchen.h"
#include "../../object/BoxFluid.h"
#include "../../XRLight.h"



bool ViewerScene::initScene()
{
	//init camera
	camera = new XRCamera();
	camera->init();

	//init light
	light = new XRPointLight();
	light->ambient = glm::vec3(1.0, 1.0, 1.0);
	light->diffuse = glm::vec3(1.0, 1.0, 1.0);
	light->specular = glm::vec3(0.0, 0.0, 0.0);
	light->position = glm::vec3(0, 100, 0);

	//init game object
	{
		XRObject* testObj = new BoxFluid();
		testObj->setCamera(camera);
		addObject(testObj);
	}

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

	return true;
}

void ViewerScene::reload()
{
	deleteAllObjects();

	XRObject* testObj = new TestTriangle();
	addObject(testObj);
}

bool ViewerScene::destroyScene()
{
	//destroy camera
	camera->destroy();
	delete camera;

	//delete light
	delete(light);

	return true;
}