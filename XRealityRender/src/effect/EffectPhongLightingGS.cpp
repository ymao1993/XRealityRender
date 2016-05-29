#include "../XRCommon.h"
#include "../utils/XRShaderUtils.h"
#include "../XRShaderManager.h"
#include "EffectPhongLightingGS.h"
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
	_Material(){}

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
	_PointLight(){}
}PointLight;

#pragma endregion

//Vertex Attributes
enum{ VPOS, VNORMAL };

bool EffectPhongLightingGS::initEffect()
{

	program = XRShaderManger::getShaderProgram(XRShaderManger::XR_SHADER_PROGRAM_PHONGLIGHTING_GS);

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

#pragma region setup normal attribute
	{
		//create and initialize normals vbo
		glGenBuffers(1, &vbo_normal);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
		XRMesh* mesh = (XRMesh*)object->getComponent(XR_COMPONENT_MESH);
		glBufferStorage(GL_ARRAY_BUFFER, sizeof(GLfloat)* 3 * mesh->vertexNum, mesh->normals, GL_MAP_WRITE_BIT);

		//binding vertex attribute with vertex buffer object
		glVertexAttribBinding(VNORMAL, 1);
		glBindVertexBuffer(1, vbo_normal, 0, sizeof(GLfloat)* 3);
		glVertexAttribFormat(VNORMAL, 3, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexAttribArray(VNORMAL);
	}

#pragma endregion

#pragma region setup material uniform block
	{
		//create, bind and initialize material ubo (bind to binding position 0 of the uniform buffer bindings)

		XRMaterial* xrmaterial = (XRMaterial*)object->getComponent(XR_COMPONENT_MATERIAL);

		Material material(xrmaterial->ambient,
						  xrmaterial->diffuse,
					      xrmaterial->specular,
						  xrmaterial->specularPower);


		glGenBuffers(1, &uboMaterial);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboMaterial);
		glBufferStorage(GL_UNIFORM_BUFFER, sizeof(material), &material, GL_MAP_WRITE_BIT);
	}
#pragma endregion

#pragma region setup light uniform block
	{
		//initialize light 

		XRScene* scene = XREngine::instance()->getScene();
		XRPointLight *xrLight = scene->light;
		PointLight light = PointLight(xrLight->position,
									  xrLight->ambient, 
									  xrLight->diffuse,
									  xrLight->specular);

		//create, bind and initialize material ubo (bind to binding position 1 of the uniform buffer bindings)
		glGenBuffers(1, &uboLight);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboLight);
		glBufferStorage(GL_UNIFORM_BUFFER, sizeof(light), &light, GL_MAP_WRITE_BIT);
	}
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

bool EffectPhongLightingGS::updateEffect(double time)
{
	//render
	glUseProgram(program);
	glBindVertexArray(vao);
	/*do not transpose here, because glm has gracefully handled it.*/

	//update uniforms
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboMaterial);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboLight);
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(object->model2World));
	glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(object->scene->camera->getWorld2View()));
	glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(object->scene->camera->getPersProj()));

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
	glBindVertexArray(0);
	glUseProgram(0);

	return true;
}

bool EffectPhongLightingGS::destroyEffect()
{
	glDeleteBuffers(1, &vao);
	glDeleteBuffers(1, &uboLight);
	glDeleteBuffers(1, &uboMaterial);
	glDeleteBuffers(1, &vbo_pos);
	glDeleteBuffers(1, &vbo_normal);
	glDeleteBuffers(1, &vbo_idx);

	return true;
}
