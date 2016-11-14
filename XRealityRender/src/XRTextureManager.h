#ifndef XRTEXTUREMANAGER_H
#define XRTEXTUREMANAGER_H

#include "XRCommon.h"

namespace XRTextureManger {

	//texture to use
	enum XR_TEXTURE_TOKEN
	{
		XR_TEXTURE_BUNNY,
		XR_TEXTURE_TERRAIN01_DISP,
		XR_TEXTURE_TERRAIN01_COLOR,

		XR_TEXTURE_ENV_BLUECLOUD,
			XR_TEXTURE_ENV_BLUECLOUD_RT,
			XR_TEXTURE_ENV_BLUECLOUD_LF,
			XR_TEXTURE_ENV_BLUECLOUD_UP,
			XR_TEXTURE_ENV_BLUECLOUD_DN,
			XR_TEXTURE_ENV_BLUECLOUD_BK,
			XR_TEXTURE_ENV_BLUECLOUD_FT,

		XR_TEXTURE_NUM,
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
