#include "../XRCommon.h"
#include "Bunny.h"
#include "../utils/XRShaderUtils.h"
#include "../XRSound.h"
#include "../effect/EffectTextureMapping.h"
#include "../effect/EffectTextureMappingInstanced.h"


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
	XRTexture *texture = new XRTexture(XRTextureManger::XR_TEXTURE_BUNNY);
	this->addComponent(texture);

	//set up effect
	vector<vec3> positions;
	for (int i = 0; i < 100; i++)
	{
		double randX = ((float) rand() / RAND_MAX - 0.5) * 20;
		double randZ = ((float) rand() / RAND_MAX - 0.5) * 20;
		positions.push_back(vec3(randX, 0, randZ));
	}
	XREffect *effect = new EffectTextureMappingInstanced(positions);
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
