#include "../XRCommon.h"
#include "Bunny.h"
#include "../utils/XRShaderUtils.h"
#include "../XRSound.h"
#include "../effect/EffectTextureMapping.h"


bool Bunny::initObject()
{
	//set position
	this->transform.position = glm::vec3(0, 0, 0);

	//set up model2world transformation
	model2World = scale(vec3(0.003,0.003,0.003));

	//set up mesh
	XRMesh *mesh = new XRMesh();
	mesh->loadMesh("res/model/bunny/bunny.obj");
	this->addComponent(mesh);

	//set up texture
	XRTexture *texture = new XRTexture("res/model/bunny/bunny.jpg");
	this->addComponent(texture);

	//set up effect
	XREffect *effect = new EffectTextureMapping();
	this->addComponent(effect);

	return true;
}

bool Bunny::updateObject(double time)
{
	return true;
}

bool Bunny::destroyObject()
{
	return true;
}
