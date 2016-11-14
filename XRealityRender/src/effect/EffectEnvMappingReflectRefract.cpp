#include "../XRCommon.h"
#include "../utils/XRShaderUtils.h"
#include "../XRShaderManager.h"
#include "EffectEnvMappingReflectRefract.h"
#include "../XREngine.h"
#include "../XRObject.h"
#include "../XRLight.h"
#include "../XRScene.h"

//Vertex Attributes
enum { VPOS, VNORMAL };

bool EffectEnvMappingReflectRefract::initEffect()
{

	program = XRShaderManger::getShaderProgram(XRShaderManger::XR_SHADER_PROGRAM_ENV_MAPPING_REFLECT_REFRACT);

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

#pragma region setup normal attribute
	{
		//create and initialize normals vbo
		glGenBuffers(1, &vbo_normal);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
		XRMesh* mesh = (XRMesh*)object->getComponent(XR_COMPONENT_MESH);
		glBufferStorage(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * mesh->vertexNum, mesh->normals, GL_MAP_WRITE_BIT);

		//binding vertex attribute with vertex buffer object
		glVertexAttribBinding(VNORMAL, 1);
		glBindVertexBuffer(1, vbo_normal, 0, sizeof(GLfloat) * 3);
		glVertexAttribFormat(VNORMAL, 3, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexAttribArray(VNORMAL);
	}

#pragma endregion

#pragma region set up index element array if needed
	{
		XRMesh* mesh = (XRMesh*)object->getComponent(XR_COMPONENT_MESH);
		if (mesh->getType() == XRMESH_TRIANGLE_SOUP_INDEXED ||
			mesh->getType() == XRMESH_TRIANGLE_STRIP_INDEXED)
		{
			glGenBuffers(1, &vbo_idx);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_idx);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)* mesh->idxNum, mesh->indices, GL_STREAM_DRAW);
		}
	}

#pragma endregion

	//by now everything is recorded in the vao
	//so we will unbind the buffer
	//and unbind the vao
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

bool EffectEnvMappingReflectRefract::updateEffect(double time)
{
	//render
	glUseProgram(program);
	glBindVertexArray(vao);
	/*do not transpose here, because glm has gracefully handled it.*/

	//bind texture cube
	XRTexture* texture = (XRTexture*)object->scene->skybox->getComponent(XR_COMPONENT_TEXTURE_CUBE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture->texture);
	glBindSampler(0, texture->sampler);

	//update uniforms
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(object->model2World));
	glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(object->scene->camera->getWorld2View()));
	glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(object->scene->camera->getPersProj()));
	glUniform3fv(3, 1, glm::value_ptr(object->scene->camera->position));
	glUniform1i(4, option); //default to reflection

	//draw
	XRMesh* mesh = (XRMesh*)object->getComponent(XR_COMPONENT_MESH);

	if (mesh->getType() == XRMESH_TRIANGLE_SOUP)
	{
		glDrawArrays(GL_TRIANGLES, 0, mesh->vertexNum);
	}
	else if (mesh->getType() == XRMESH_TRIANGLE_SOUP_INDEXED)
	{
		glDrawElements(GL_TRIANGLES, mesh->idxNum, GL_UNSIGNED_INT, 0);
	}
	else
	{
		XRDebug::logE("Mesh type not supported by program 'PhongLightingGS'");
	}

	//unbind vao/shader program
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindSampler(0, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	return true;
}

bool EffectEnvMappingReflectRefract::destroyEffect()
{
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &vbo_pos);
	glDeleteBuffers(1, &vbo_normal);
	glDeleteBuffers(1, &vbo_idx);

	return true;
}
