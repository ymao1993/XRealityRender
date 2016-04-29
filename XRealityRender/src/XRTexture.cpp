#include "XRTexture.h"
#include <SOIL.h>

bool XRTexture::loadTexture(const char* filePath)
{
	/* load an image file directly as a new OpenGL texture */
	texture = SOIL_load_OGL_texture
		(
		filePath,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

	/* check for an error during the load process */
	if (texture == 0)
	{
		XRDebug::logE("SOIL loading error: '%s'\n", SOIL_last_result());
		return false;
	}

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