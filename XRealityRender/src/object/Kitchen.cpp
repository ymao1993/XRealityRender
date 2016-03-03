#include "../gl3w/gl3w.h"
#include "Kitchen.h"
#include "../utils/XRShaderUtils.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "../XRSound.h"
#include "../effect/EffectPhongLightingGS.h"


bool Kitchen::initObject()
{
	//set position
	this->transform.position = glm::vec3(0, 0, 0);

	//set up mesh
	XRMesh *mesh = new XRMesh();
	mesh->loadMesh("res/model/dragon.obj");
	this->addComponent(mesh);

	//set up material
	XRMaterial* material = new XRMaterial(
		glm::vec3(0.3, 0.3, 0.0),
		glm::vec3(0.5, 0.5, 0.0),
		glm::vec3(1.0, 1.0, 0.5),
		10);
	this->addComponent(material);

	//set up effect
	XREffect *effect = new EffectPhongLightingGS();
	this->addComponent(effect);

	//set up sound
	XRSound *sound = new XRSound();
	sound->loadSound("music", "res/sound/music.mp3");
	sound->loadSound("cow", "res/sound/cow.wav");
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
		sound->playSound("cow");
		//sound->playSound("music");
		startPlay = true;
	}

	return true;
}

bool Kitchen::destroyObject()
{
	return true;
}

bool Kitchen::render()
{
	return true;
}