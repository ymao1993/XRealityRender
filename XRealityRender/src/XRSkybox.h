#ifndef XRSKYBOX_H
#define XRSKYBOX_H

#include "XRObject.h"
#include "XRTexture.h"
#include "XRTextureManager.h"

/**
* XRSkybox
* XRSkybox simulate the environment via environment mapping
*
* @Author Yu Mao
*/
class XRSkybox : public XRObject
{
public:

	//Constructor
	XRSkybox() :XRObject("Skybox") {}

	//extended from XREntity
	virtual bool initObject() final;
	virtual bool updateObject(double time) final;
	virtual bool destroyObject() final;

protected:
	XRTextureCube* texture;
};

#endif