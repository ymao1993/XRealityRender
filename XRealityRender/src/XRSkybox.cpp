#include "XRSkybox.h"

#include "effect\EffectSkybox.h"
#include "XRMeshGenerator.h"
#include "XRTextureManager.h"

bool XRSkybox::initObject()
{
	XREffect* effect = new EffectSkybox();
	this->addComponent(effect);

	XRMesh * mesh = new XRMesh();
	XRMeshGenerator::generateCube(*mesh, vec3(0.f, 0.f, 0.f), 100.f, 100.f, 100.f);
	this->addComponent(mesh);

	XRTexture* SkyboxTexture = new XRTextureCube(XRTextureManger::XR_TEXTURE_ENV_BLUECLOUD);
	this->addComponent(SkyboxTexture);

	CHECK_GL_ERROR;

	return true;
}

bool XRSkybox::updateObject(double time)
{
	if (XRDevice::isKeyPressed(GLFW_KEY_SPACE))
	{
		XREffect* effect = (XREffect*)this->getComponent(XR_COMPONENT_EFFECT);
		effect->setSkeletonMode(true);
	}
	else
	{
		XREffect* effect = (XREffect*)this->getComponent(XR_COMPONENT_EFFECT);
		effect->setSkeletonMode(false);
	}

	return true;
}

bool XRSkybox::destroyObject()
{
	return true;
}