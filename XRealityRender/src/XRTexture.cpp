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
	glGenSamplers(1, &sampler);
	return;
}

bool XRTexture::destroy()
{
	glDeleteSamplers(1, &sampler);
	glDeleteTextures(1, &texture);
	return true;
}