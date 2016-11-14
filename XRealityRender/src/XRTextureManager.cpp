#include "XRTextureManager.h"
#include <map>
#include <SOIL.h>

namespace XRTextureManger {

	//DATA
	static const string TEXTURE_FOLDER_PATH = "./res/texture/";

	//texture token->name map
	//Note: for texture cube, only the texture cube itself will be loaded as texture,
	//images of each face will not be loaded as texture, but they will be loaded to 
	//initialize the corresponding texture cube.
	static const std::map<XR_TEXTURE_TOKEN, string> textureNames =
	{
		{XR_TEXTURE_BUNNY, "bunny.jpg"},
		{XR_TEXTURE_TERRAIN01_DISP, "terrain01_disp.jpg"},
		{XR_TEXTURE_TERRAIN01_COLOR, "terrain01_color.jpg"},

		{ XR_TEXTURE_ENV_BLUECLOUD, "env_bluecloud/"},
			{ XR_TEXTURE_ENV_BLUECLOUD_RT, "bluecloud_rt.jpg" },
			{ XR_TEXTURE_ENV_BLUECLOUD_LF, "bluecloud_lf.jpg" },
			{ XR_TEXTURE_ENV_BLUECLOUD_UP, "bluecloud_up.jpg" },
			{ XR_TEXTURE_ENV_BLUECLOUD_DN, "bluecloud_dn.jpg" },
			{ XR_TEXTURE_ENV_BLUECLOUD_BK, "bluecloud_bk.jpg" },
			{ XR_TEXTURE_ENV_BLUECLOUD_FT, "bluecloud_ft.jpg" },

	};

	static std::map<XR_TEXTURE_TOKEN, GLuint> textures;

	//forward declaration
	static GLuint loadTexture(const char* filePath);
	static GLuint loadTextureCube(vector<string> faceFiles);

	//init all textures
	void init()
	{
		for (auto iter = textureNames.begin(); iter != textureNames.end(); iter++)
		{
			GLuint texture;
			string filePath = TEXTURE_FOLDER_PATH + iter->second;
			if (filePath[filePath.length() - 1] == '/')
			{
				vector<string> files;
				auto iter_tmp = iter;
				for (int i = 0; i < 6; i++)
				{
					iter++;
					files.push_back(filePath + iter->second);
				}
				texture = loadTextureCube(files);
				textures[iter_tmp->first] = texture;
			}
			else
			{
				texture = loadTexture(filePath.c_str());
				textures[iter->first] = texture;
			}
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

	static GLuint loadTextureCube(vector<string> faceFiles)
	{
		GLuint cube;
		glGenTextures(1, &cube);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cube);
		unsigned char* image;
		for (int i = 0; i < 6; i++)
		{
			int width, height, channels;
			const char* filePath = faceFiles[i].c_str();
			image = SOIL_load_image(filePath, &width, &height, &channels, SOIL_LOAD_RGB);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			std::free(image);
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		return cube;
	}

	static GLuint loadTexture(const char* filePath)
	{
		int length = strlen(filePath);
		if (filePath[length - 1] == '/')
		{
			GLuint texture;
			glGenTextures(1, &texture);
			return texture;
		}
		else
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
}