#include "../XRCommon.h"
#include "Kitchen.h"
#include "../utils/XRShaderUtils.h"
#include "../XRSound.h"
#include "../effect/EffectPhongLightingGS.h"


bool Kitchen::initObject()
{
	//set position
	this->transform.position = glm::vec3(0, 0, 0);

	//set up model2world transformation
	model2World = glm::translate(glm::vec3(0,-1.65,0)) * glm::rotate(30.f, glm::vec3(1,0,0));

	//set up mesh
	XRMesh *mesh = new XRMesh();
	mesh->loadMesh("res/model/kitchen.obj");
	this->addComponent(mesh);

	//set up material
	XRMaterial* material = new XRMaterial(
		glm::vec3(0.3, 0.3, 0.3),
		glm::vec3(0.3, 0.3, 0.3),
		glm::vec3(0.8, 0.8, 0.8),
		40);
	this->addComponent(material);

	//set up effect
	XREffect *effect = new EffectPhongLightingGS();
	this->addComponent(effect);

	//set up sound
	XRSound *sound = new XRSound();
	sound->loadSound("frying", "res/sound/frying.mp3");
	this->addComponent(sound);
	return true;
}

bool Kitchen::updateObject(double time)
{
	//play sound at the first frame
	static bool startPlay = false;
	if (!startPlay)
	{
		XRSound* sound = (XRSound*)getComponent(XR_COMPONENT_SOUND);
		sound->playSound("frying");
		startPlay = true;
	}

	return true;
}

bool Kitchen::destroyObject()
{
	return true;
}
