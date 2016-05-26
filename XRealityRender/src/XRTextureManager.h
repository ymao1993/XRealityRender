#ifndef XRTEXTUREMANAGER_H
#define XRTEXTUREMANAGER_H

#include "XRCommon.h"

namespace XRTextureManger {

	//texture to use
	enum XR_TEXTURE_TOKEN
	{
		XR_TEXTURE_BUNNY,
	};

	//init all textures
	void init();

	//get texture
	GLuint getTexture(XR_TEXTURE_TOKEN token);

	//delete texture
	void deleteTexture(XR_TEXTURE_TOKEN token);

	//delete all the resources
	void free();
}

#endif
