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

class XRTexture : public XRComponent
{
public:

	XRTexture(const char* filePath) :XRComponent(XR_COMPONENT_TEXTURE)
	{
		loadTexture(filePath);
		initSampler();
	}

	GLuint texture;
	GLuint sampler;

private:
	virtual bool init(){ return true; };
	virtual bool update(double time){ return true; }
	virtual bool destroy();
	bool loadTexture(const char* filePath);
	void initSampler();
};


#endif