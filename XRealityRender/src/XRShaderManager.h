#ifndef XRSHADERMANAGER_H
#define XRSHADERMANAGER_H

#include "XRCommon.h"
#include "XRShaderDescriptor.h"

namespace XRShaderManger{

	//shader programs to use
	enum XR_SHADER_PROGRAM_TOKEN
	{
		XR_SHADER_PROGRAM_SINGLE_COLOR,
		XR_SHADER_PROGRAM_TEXTURE_MAPPING,
		XR_SHADER_PROGRAM_TEXTURE_MAPPING_INSTANCED,
		XR_SHADER_PROGRAM_PHONGLIGHTING_GS,
		XR_SHADER_PROGRAM_TERRAIN_RENDERING,
	};

	//init all shaders
	void init(bool binary = false);

	//get monolithic shader program
	GLuint getShaderProgram(XR_SHADER_PROGRAM_TOKEN token);

	//get pre-configured shader pipeline
	GLuint getShaderPipeline(XR_SHADER_PROGRAM_TOKEN token);

	//delete monolithic shader program
	void deleteShaderProgram(XR_SHADER_PROGRAM_TOKEN token);

	//get all descriptors (used for inspector)
	void getAllDescriptors(vector<ProgramDesciptor>& descriptor);

	//delete all the resources
	void free();

}

#endif