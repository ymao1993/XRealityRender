#include "../../XRCommon.h"
#include "EnvironmentScene.h"
#include "EnvironmentSceneGUI.h"
#include "../../XRLight.h"

#include "../../object/bunny.h"
#include "../../object/Dragon.h"
#include "../../XRSkybox.h"


bool EnvironmentScene::initScene()
{
	//init light
	light = new XRPointLight();
	light->ambient = glm::vec3(1.0, 1.0, 1.0);
	light->diffuse = glm::vec3(1.0, 1.0, 1.0);
	light->specular = glm::vec3(0.0, 0.0, 0.0);
	light->position = glm::vec3(0, 100, 0);

	//init game object
	{
		XRObject* obj = new Dragon();
		this->addObject(obj);
	}
	{
		XRObject* obj = new XRSkybox();
		this->addObject(obj);
	}
	return true;
}

XRUserInterface* EnvironmentScene::createUserInterface()
{
	return new EnvrionmentSceneGUI(this);
}

bool EnvironmentScene::updateScene(double time)
{

	return true;
}

bool EnvironmentScene::destroyScene()
{
	//destroy camera
	camera->destroy();
	delete camera;

	//delete light
	delete(light);

	return true;
}