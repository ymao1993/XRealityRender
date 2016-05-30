#include "../XRCommon.h"
#include "Terrain.h"
#include "../utils/XRShaderUtils.h"
#include "../effect/EffectTerrainRendering.h"
#include "../XRMeshGenerator.h"
#include "../XRTextureManager.h"


bool Terrain::initObject()
{
	//set position
	this->transform.position = glm::vec3(0, 0, 0);

	//set up material
	XRMaterial* material = new XRMaterial(
		glm::vec3(0.3, 0.3, 0.3),
		glm::vec3(0.3, 0.3, 0.3),
		glm::vec3(0.8, 0.8, 0.8),
		40);
	this->addComponent(material);

	//set up mesh
	XRMesh *mesh = new XRMesh();
	XRMeshGenerator::generateHorizontalPlane(*mesh, center, numCell.x, numCell.y, cellSize);
	this->addComponent(mesh);

	//set up displacement map
	XRTexture *displacementMap = new XRTextureDisp(XRTextureManger::XR_TEXTURE_TERRAIN01_DISP);
	this->addComponent(displacementMap);

	//set up color map
	XRTexture *colorMap = new XRTextureColor(XRTextureManger::XR_TEXTURE_TERRAIN01_COLOR);
	this->addComponent(colorMap);

	//set up effect
	XREffect *effect = new TerrainRendering();
	effect->setSkeletonMode(false);
	this->addComponent(effect);

	return true;
}

bool Terrain::updateObject(double time)
{
	XREffect *effect = (XREffect*) this->getComponent(XR_COMPONENT_EFFECT);
	if (XRDevice::isKeyPressed(GLFW_KEY_SPACE))
	{
		effect->setSkeletonMode(true);
	}
	else
	{
		effect->setSkeletonMode(false);
	}

	return true;
}

bool Terrain::destroyObject()
{
	return true;
}
