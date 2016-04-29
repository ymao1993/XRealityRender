#include "../XRCommon.h"
#include "EffectTextureMapping.h"
#include "../utils/XRShaderUtils.h"
#include "../XREngine.h"
#include "../XRObject.h"
#include "../XRLight.h"
#include "../XRScene.h"


#define XR_ASSIGN_GLM(NAME) \
	memcpy(this->NAME, glm::value_ptr(NAME), sizeof(this->NAME))
#define XR_ASSIGN(NAME) \
	this->NAME = NAME;

//Vertex Attributes
enum{ VPOS, VTXCOORDS };

bool EffectTextureMapping::initEffect()
{

#pragma region build shaders
	{
		//load shaders
		GLuint shaders[2];
		shaders[0] = XRShaderUtils::loadShader("res/shader/TextureMapping/TextureMapping.vs.glsl", GL_VERTEX_SHADER);
		shaders[1] = XRShaderUtils::loadShader("res/shader/TextureMapping/TextureMapping.fs.glsl", GL_FRAGMENT_SHADER);
		//link program
		program = XRShaderUtils::linkShaderProgram(shaders, 2, true);
	}

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
		glBufferStorage(GL_ARRAY_BUFFER, sizeof(GLfloat)* 3 * mesh->vertexNum, mesh->positions, GL_MAP_WRITE_BIT);

		//binding vertex attribute with vertex buffer object
		glVertexAttribBinding(VPOS, 0);
		glBindVertexBuffer(0, vbo_pos, 0, sizeof(GLfloat)* 3);
		glVertexAttribFormat(VPOS, 3, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexAttribArray(VPOS);
	}

#pragma endregion

#pragma region setup txcoords attribute
	{
		//create and initialize normals vbo
		glGenBuffers(1, &vbo_txcoords);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_txcoords);
		XRMesh* mesh = (XRMesh*)object->getComponent(XR_COMPONENT_MESH);
		glBufferStorage(GL_ARRAY_BUFFER, sizeof(GLfloat)* 2 * mesh->vertexNum, mesh->txcoords, GL_MAP_WRITE_BIT);

		//binding vertex attribute with vertex buffer object
		glVertexAttribBinding(VTXCOORDS, 1);
		glBindVertexBuffer(1, vbo_txcoords, 0, sizeof(GLfloat) * 2);
		glVertexAttribFormat(VTXCOORDS, 2, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexAttribArray(VTXCOORDS);
	}

#pragma endregion

	//by now everything is recorded in the vao
	//so we will unbind the buffer
	//and unbind the vao
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

enum{
	UNIFORM_TRANSFORMATION,
	UNIFORM_SAMPLER2D
};

bool EffectTextureMapping::updateEffect(double time)
{
	//render
	glUseProgram(program);
	glBindVertexArray(vao);

	//bind texture to texture2D binding target
	XRTexture* texture = (XRTexture*)object->getComponent(XR_COMPONENT_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, texture->texture);

	//update uniforms
	glUniformMatrix4fv(UNIFORM_TRANSFORMATION, 1, GL_FALSE, glm::value_ptr(object->camera->getPersProj() * object->camera->getWorld2View() * object->model2World));

	//draw
	XRMesh* mesh = (XRMesh*)object->getComponent(XR_COMPONENT_MESH);
	glDrawArrays(GL_TRIANGLES, 0, mesh->vertexNum);

	//unbind vao/shader program
	glBindSampler(0, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	return true;
}

bool EffectTextureMapping::destroyEffect()
{
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &vbo_pos);
	glDeleteBuffers(1, &vbo_txcoords);

	return true;
}
