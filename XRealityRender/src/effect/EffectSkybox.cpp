#include "../XRCommon.h"
#include "EffectSkybox.h"
#include "../XRShaderManager.h"
#include "../utils/XRShaderUtils.h"
#include "../XREngine.h"
#include "../XRObject.h"
#include "../XRLight.h"
#include "../XRScene.h"

//Vertex Attributes
enum { VPOS };

bool EffectSkybox::initEffect()
{
	program = XRShaderManger::getShaderProgram(XRShaderManger::XR_SHADER_PROGRAM_ENVIRONMENT_MAPPING);

#pragma endregion

	//create and initialize vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

#pragma region setup position attribute
	{
		//create and initialize position vbo_pos
		glGenBuffers(1, &vbo_pos);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
		XRMesh* mesh = (XRMesh*)object->getComponent(XR_COMPONENT_MESH);
		glBufferStorage(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * mesh->vertexNum, mesh->positions, GL_MAP_WRITE_BIT);

		//binding vertex attribute with vertex buffer object
		glVertexAttribBinding(VPOS, 0);
		glBindVertexBuffer(0, vbo_pos, 0, sizeof(GLfloat) * 3);
		glVertexAttribFormat(VPOS, 3, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexAttribArray(VPOS);
	}

#pragma endregion


#pragma endregion

	//by now everything is recorded in the vao
	//so we will unbind the buffer
	//and unbind the vao
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

enum {
	UNIFORM_W2C,
	UNIFORM_ENV_CUBE,
};

bool EffectSkybox::updateEffect(double time)
{
	//render
	glUseProgram(program);
	glBindVertexArray(vao);

	//bind texture to binding target
	XRTexture* texture = (XRTexture*)object->getComponent(XR_COMPONENT_TEXTURE_CUBE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture->texture);
	glBindSampler(0, texture->sampler);

	//update uniforms

	/* The skybox should not translate with the camera,
	 * so we only keeps the rotation component
	 */
	mat4 rotation = object->scene->camera->getWorld2View(); 
	rotation[3][0] = 0;
	rotation[3][1] = 0;
	rotation[3][2] = 0;

	glUniformMatrix4fv(UNIFORM_W2C, 1, GL_FALSE, glm::value_ptr(object->scene->camera->getPersProj() * rotation));

	//draw
	XRMesh* mesh = (XRMesh*)object->getComponent(XR_COMPONENT_MESH);

	if (mesh->getType() == XRMESH_TRIANGLE_SOUP_INDEXED)
	{
		glDrawElements(GL_TRIANGLES, mesh->idxNum, GL_UNSIGNED_INT, mesh->indices);
	}
	else
	{
		XRDebug::logE("Mesh type not supported by program 'EnvironmentMapping'");
	}

	//unbind vao/shader program
	glBindSampler(0, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	return true;
}

bool EffectSkybox::destroyEffect()
{
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &vbo_pos);

	return true;
}
