#include "../../XRCommon.h"
#include "ForestScene.h"
#include "ForestSceneGUI.h"
#include "../../XRLight.h"

#include "../../object/PointCloud.h"
#include "../../object/Kitchen.h"
#include "../../object/BoxFluid.h"
#include "../../object/bunny.h"


bool ForestScene::initScene()
{
	//init light
	light = new XRPointLight();
	light->ambient = glm::vec3(1.0, 1.0, 1.0);
	light->diffuse = glm::vec3(1.0, 1.0, 1.0);
	light->specular = glm::vec3(0.0, 0.0, 0.0);
	light->position = glm::vec3(0, 100, 0);

	//init game object

	return true;
}

XRUserInterface* ForestScene::createUserInterface()
{
	return new ForestSceneGUI(this);
}

bool ForestScene::updateScene(double time)
{

	return true;
}

bool ForestScene::destroyScene()
{
	//destroy camera
	camera->destroy();
	delete camera;

	//delete light
	delete(light);

	return true;
}