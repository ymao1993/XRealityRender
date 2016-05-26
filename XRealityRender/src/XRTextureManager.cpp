#include "XRTextureManager.h"
#include <map>
#include <SOIL.h>

namespace XRTextureManger {

	//DATA
	static const string TEXTURE_FOLDER_PATH = "./res/texture/";

	static const std::map<XR_TEXTURE_TOKEN, string> textureNames =
	{
		{XR_TEXTURE_BUNNY, "bunny.jpg"},
	};

	static std::map<XR_TEXTURE_TOKEN, GLuint> textures;

	//forward declaration
	static GLuint loadTexture(const char* filePath);

	//init all textures
	void init()
	{
		for (auto iter = textureNames.begin(); iter != textureNames.end(); iter++)
		{
			string filePath = "./res/texture/bunny.jpg";
			GLuint texture = loadTexture(filePath.c_str());
			textures[iter->first] = texture;
		}
	}

	//get texture
	GLuint getTexture(XR_TEXTURE_TOKEN token)
	{
		if (textures.find(token) != textures.end())
		{
			return textures[token];
		}
		return 0;
	}

	//delete texture
	void deleteTexture(XR_TEXTURE_TOKEN token)
	{
		if (textures.find(token) != textures.end())
		{
			glDeleteTextures(1, &textures[token]);
			textures.erase(token);
		}
	}

	//delete all the resources
	void free()
	{
		for (auto iter = textures.begin(); iter != textures.end(); iter++)
		{
			glDeleteTextures(1, &iter->second);
		}
		textures.clear();
	}

	static GLuint loadTexture(const char* filePath)
	{
		/* load an image file directly as a new OpenGL texture */
		GLuint texture = SOIL_load_OGL_texture
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
			return 0;
		}

		return texture;
	}
}