#include "../../XRCommon.h"
#include "ViewerScene.h"
#include "ViewerSceneGUI.h"
#include "../../XRLight.h"

#include "../../object/PointCloud.h"
#include "../../object/Kitchen.h"
#include "../../object/BoxFluid.h"
#include "../../object/bunny.h"



bool ViewerScene::initScene()
{
	//init light
	light = new XRPointLight();
	light->ambient = glm::vec3(1.0, 1.0, 1.0);
	light->diffuse = glm::vec3(1.0, 1.0, 1.0);
	light->specular = glm::vec3(0.0, 0.0, 0.0);
	light->position = glm::vec3(0, 100, 0);

	//init game object
	{
		XRObject* testObj = new Bunny();
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
	brush.drawBox(vec3(0, 0, 0), 5, 5, 5);
	brush.drawBox(vec3(0, 0, 0), 4, 6, 7);
	brush.drawBox(vec3(0, 0, 0), 1, 3, 2);

	brush.drawTriangle(vec3(10, 1, 0), vec3(0, 11, 0), vec3(-10, 1, 0));

	brush.drawLine(vec3(0, 0, 0), vec3(0, 1, 0));
	brush.drawLine(vec3(0, 1, 0), vec3(1, 0, 0));

	//static double offset = 0;
	//brush.drawLine(vec3(offset, 0, 0), vec3(offset + time, 0, 0), false);
	//offset += time;

	return true;
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