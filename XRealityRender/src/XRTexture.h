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

protected:

	XRTexture(XRTextureManger::XR_TEXTURE_TOKEN token, XRComponentType type) :XRComponent(type)
	{
		loadTexture(token);
		initSampler();
	}

private:
	virtual bool init(){ return true; };
	virtual bool update(double time){ return true; }
	virtual bool destroy();
	bool loadTexture(XRTextureManger::XR_TEXTURE_TOKEN token);
	void initSampler();
};

class XRTextureDisp : public XRTexture
{
public:
	XRTextureDisp(XRTextureManger::XR_TEXTURE_TOKEN token):XRTexture(token, XR_COMPONENT_TEXTURE_DISPLACEMENT){}
};

class XRTextureColor : public XRTexture
{
public:
	XRTextureColor(XRTextureManger::XR_TEXTURE_TOKEN token) :XRTexture(token, XR_COMPONENT_TEXTURE_COLOR) {}
};


#endif