#include "../XRCommon.h"
#include "EffectTextureMappingInstanced.h"
#include "../XRShaderManager.h"
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
enum{ VPOS, VTXCOORDS, INSTANCED_POS};

bool EffectTextureMappingInstanced::initEffect()
{
	program = XRShaderManger::getShaderProgram(XRShaderManger::XR_SHADER_PROGRAM_TEXTURE_MAPPING_INSTANCED);

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
		glBindVertexBuffer(1, vbo_txcoords, 0, sizeof(GLfloat)* 2);
		glVertexAttribFormat(VTXCOORDS, 2, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexAttribArray(VTXCOORDS);
	}

#pragma endregion


#pragma region setup instanced_position attribute
	{
		float* instancedPos = new float[instanced_positions.size() * 3];
		for (int i = 0; i < instanced_positions.size(); i++)
		{
			instancedPos[i * 3 + 0] = instanced_positions[i].x;
			instancedPos[i * 3 + 1] = instanced_positions[i].y;
			instancedPos[i * 3 + 2] = instanced_positions[i].z;
		}

		glGenBuffers(1, &vbo_instanced_pos);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_instanced_pos);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* instanced_positions.size() * 3, instancedPos, GL_STREAM_DRAW);

		delete[] instancedPos;

		//bind vertex attributes to the buffer
		glVertexAttribBinding(INSTANCED_POS, 2);
		glBindVertexBuffer(2, vbo_instanced_pos, 0, sizeof(float)* 3);
		glVertexAttribFormat(INSTANCED_POS, 3, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexAttribArray(INSTANCED_POS);
		//divisor is the number of instances you would like to pass between each new value is read from the instanced array
		glVertexAttribDivisor(INSTANCED_POS, 1); 
	}


#pragma region setup indices element array
	{
		XRMesh* mesh = (XRMesh*)object->getComponent(XR_COMPONENT_MESH);
		if (mesh->getType() == XRMESH_TRIANGLE_SOUP_INDEXED)
		{
			glGenBuffers(1, &vbo_idx);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_idx);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)* mesh->faceNum * 3, mesh->indices, GL_STREAM_DRAW);
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

enum{
	UNIFORM_M2W,
	UNIFORM_W2C,
	UNIFORM_SAMPLER2D
};

bool EffectTextureMappingInstanced::updateEffect(double time)
{
	//render
	glUseProgram(program);
	glBindVertexArray(vao);

	//bind texture to texture2D binding target
	XRTexture* texture = (XRTexture*)object->getComponent(XR_COMPONENT_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, texture->texture);

	//update uniforms
	glUniformMatrix4fv(UNIFORM_M2W, 1, GL_FALSE, glm::value_ptr(object->model2World));
	glUniformMatrix4fv(UNIFORM_W2C, 1, GL_FALSE, glm::value_ptr(object->scene->camera->getPersProj() * object->scene->camera->getWorld2View()));

	//draw
	XRMesh* mesh = (XRMesh*)object->getComponent(XR_COMPONENT_MESH);

	if (mesh->getType() == XRMESH_TRIANGLE_SOUP)
	{
		glDrawArraysInstanced(GL_TRIANGLES, 0, mesh->vertexNum, instanced_positions.size());
	}
	else // mesh->getType() == XRMESH_TRIANGLE_SOUP_INDEXED
	{
		glDrawElementsInstanced(GL_TRIANGLES, mesh->faceNum * 3, GL_UNSIGNED_INT, 0, instanced_positions.size());
	}

	//unbind vao/shader program
	glBindSampler(0, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	return true;
}

bool EffectTextureMappingInstanced::destroyEffect()
{
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &vbo_pos);
	glDeleteBuffers(1, &vbo_txcoords);
	glDeleteBuffers(1, &vbo_idx);

	return true;
}
