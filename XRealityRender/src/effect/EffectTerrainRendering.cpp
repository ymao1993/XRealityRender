#include "../XRCommon.h"
#include "../utils/XRShaderUtils.h"
#include "../XRShaderManager.h"
#include "EffectTerrainRendering.h"
#include "../XREngine.h"
#include "../XRObject.h"
#include "../XRLight.h"
#include "../XRScene.h"


#define XR_ASSIGN_GLM(NAME) \
	memcpy(this->NAME, glm::value_ptr(NAME), sizeof(this->NAME))
#define XR_ASSIGN(NAME) \
	this->NAME = NAME;

#pragma region uniform block structs

//layout(std140, binding = 1) uniform Material{
//	vec3  ambient;
//	vec3  diffuse;
//	vec3  specular;
//	float spower;
//}material;
typedef struct _Material
{
	float ambient[3];	float _pad1;
	float diffuse[3];	float _pad2;
	float specular[3];  float spower;


	_Material(const glm::vec3 &ambient,
		const glm::vec3 &diffuse,
		const glm::vec3 &specular,
		float spower)
	{
		XR_ASSIGN_GLM(ambient);
		XR_ASSIGN_GLM(diffuse);
		XR_ASSIGN_GLM(specular);
		XR_ASSIGN(spower);
	}
	_Material() {}

}Material;

//layout(std140, binding = 0) uniform PointLight{
//	vec3  pos;
//	vec3  ambient;
//	vec3  diffuse;
//	vec3  specular;
//}pointLight;
typedef struct _PointLight
{
	float pos[3];		float _pad0;
	float ambient[3];	float _pad1;
	float diffuse[3];	float _pad2;
	float specular[3];  float _pad3;

	_PointLight(const glm::vec3 &pos,
		const glm::vec3 &ambient,
		const glm::vec3 &diffuse,
		const glm::vec3 &specular)
	{
		XR_ASSIGN_GLM(pos);
		XR_ASSIGN_GLM(ambient);
		XR_ASSIGN_GLM(diffuse);
		XR_ASSIGN_GLM(specular);
	}
	_PointLight() {}
}PointLight;

#pragma endregion

//Vertex Attributes
enum { VPOS, VTEXCOORD };

bool TerrainRendering::initEffect()
{

	program = XRShaderManger::getShaderProgram(XRShaderManger::XR_SHADER_PROGRAM_TERRAIN_RENDERING);

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
	glBindBuffer(GL_ARRAY_BUFFER, 0);

#pragma endregion

#pragma region set up texture coordinate attribtue
	{
		glGenBuffers(1, &vbo_txc);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_txc);
		XRMesh* mesh = (XRMesh*)object->getComponent(XR_COMPONENT_MESH);
		glBufferStorage(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * mesh->vertexNum, mesh->txcoords, GL_MAP_WRITE_BIT);

		glVertexAttribBinding(VTEXCOORD, 1);
		glBindVertexBuffer(1, vbo_txc, 0, sizeof(GLfloat) * 2);
		glVertexAttribFormat(VTEXCOORD, 2, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexAttribArray(VTEXCOORD);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#pragma endregion

#pragma region set up index element array if needed
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

bool TerrainRendering::updateEffect(double time)
{
	//render
	glUseProgram(program);
	glBindVertexArray(vao);

	//update uniforms
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(object->scene->camera->getWorld2Clip() * object->model2World));

	//bind multiple texture to texture units
	XRTexture* texture_disp = (XRTexture*)object->getComponent(XR_COMPONENT_TEXTURE_DISPLACEMENT);
	glBindTextureUnit(0, texture_disp->texture);
	glBindSampler(0, texture_disp->sampler);

	XRTexture* texture_color = (XRTexture*)object->getComponent(XR_COMPONENT_TEXTURE_COLOR);
	glBindTextureUnit(1, texture_color->texture);
	glBindSampler(0, texture_color->sampler);

	//draw
	XRMesh* mesh = (XRMesh*)object->getComponent(XR_COMPONENT_MESH);

	if (mesh->getType() == XRMESH_TRIANGLE_SOUP)
	{
		glDrawArrays(GL_TRIANGLES, 0, mesh->vertexNum);
	}
	else // mesh->getType() == XRMESH_TRIANGLE_SOUP_INDEXED
	{
		glDrawElements(GL_TRIANGLES, mesh->faceNum * 3, GL_UNSIGNED_INT, 0);
	}

	//unbind vao/shader program
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTextureUnit(0, 0);
	glBindTextureUnit(1, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	return true;
}

bool TerrainRendering::destroyEffect()
{
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &vbo_pos);
	glDeleteBuffers(1, &vbo_idx);
	glDeleteBuffers(1, &vbo_txc);

	return true;
}
