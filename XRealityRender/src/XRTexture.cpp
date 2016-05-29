#include "XRTexture.h"
#include "XRTextureManager.h"
#include <SOIL.h>

bool XRTexture::loadTexture(XRTextureManger::XR_TEXTURE_TOKEN token)
{
	texture = XRTextureManger::getTexture(token);
	glBindTexture(GL_TEXTURE_2D, texture);

	return true;
}

void XRTexture::initSampler()
{
	//by default, generate linear filter
	glGenSamplers(1, &sampler);
	glSamplerParameteri(sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	return;
}

bool XRTexture::destroy()
{
	glDeleteSamplers(1, &sampler);
	glDeleteTextures(1, &texture);
	return true;
}