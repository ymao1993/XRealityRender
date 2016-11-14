#include "../XRCommon.h"
#include "Dragon.h"
#include "../utils/XRShaderUtils.h"
#include "../XRSound.h"
#include "../effect/EffectTextureMapping.h"
#include "../effect/EffectTextureMappingInstanced.h"
#include "../effect/EffectEnvMappingReflectRefract.h"


bool Dragon::initObject()
{
	//set position
	this->transform.position = glm::vec3(0, 0, 0);

	//set up model2world transformation
	model2World = translate(vec3(0, 0, 9)) * rotate(90.f, vec3(0, 1, 0));

	//set up mesh
	XRMesh *mesh = new XRMesh();
	mesh->loadMesh("res/model/dragon.obj");
	this->addComponent(mesh);

	XREffect *effect = new EffectEnvMappingReflectRefract(1);
	this->addComponent(effect);

	return true;
}

bool Dragon::updateObject(double time)
{
	return true;
}

bool Dragon::destroyObject()
{
	return true;
}
