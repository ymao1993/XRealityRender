/**
 * XRTexture represents the texture of an object.
 *
 * @Author Yu Mao
 */
#ifndef XRTEXTURE_H
#define XRTEXTURE_H

#include "XRCommon.h"
#include "XRComponent.h"
#include "XRComponentType.h"

#include "XRTextureManager.h"

class XRTexture : public XRComponent
{
public:

	XRTexture(XRTextureManger::XR_TEXTURE_TOKEN token) :XRComponent(XR_COMPONENT_TEXTURE)
	{
		loadTexture(token);
		initSampler();
	}

	GLuint texture;
	GLuint sampler;

private:
	virtual bool init(){ return true; };
	virtual bool update(double time){ return true; }
	virtual bool destroy();
	bool loadTexture(XRTextureManger::XR_TEXTURE_TOKEN token);
	void initSampler();
};


#endif