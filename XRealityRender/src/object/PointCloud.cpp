#include "../XRCommon.h"
#include "PointCloud.h"
#include "../utils/XRShaderUtils.h"
#include "../utils/XRPointCloudUtils.h"
#include "../XRCommon.h"
#include "../XRScene.h"


bool PointCloud::initObject()
{
#pragma region build shaders
	{
		//load shaders
		GLuint shaders[2];
		shaders[0] = XRShaderUtils::loadShader("res/shader/PhongLighting(GS)/phongLighting(gs).vs.glsl", GL_VERTEX_SHADER);
		shaders[1] = XRShaderUtils::loadShader("res/shader/PhongLighting(GS)/phongLighting(gs).fs.glsl", GL_FRAGMENT_SHADER);
		//link program
		program = XRShaderUtils::linkShaderProgram(shaders, 2, true);
	}

#pragma endregion

	//load model
	GLfloat *positions = NULL;
	GLfloat *normals = NULL;
	pointNum = 0;
	XRDebug::log("start reading point cloud...");
	XRPointCloudUtils::readPts("res/model/room.ply", pointNum, &positions, &normals);
	XRDebug::log("reading complete.");
	printf("%d points loaded", pointNum);

	//create and initialize vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Vertex Attributes
	enum{ VPOS, VNORMAL };

#pragma region setup position attribute
	{
		//create and initialize position vbo_pos
		GLuint vbo_pos;
		glGenBuffers(1, &vbo_pos);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
		glBufferStorage(GL_ARRAY_BUFFER, sizeof(GLfloat)* 3 * pointNum, positions, GL_MAP_WRITE_BIT);

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
		GLuint vbo_normal;
		glGenBuffers(1, &vbo_normal);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
		glBufferStorage(GL_ARRAY_BUFFER, sizeof(GLfloat)* 3 * pointNum, normals, GL_MAP_WRITE_BIT);

		//binding vertex attribute with vertex buffer object
		glVertexAttribBinding(VNORMAL, 1);
		glBindVertexBuffer(1, vbo_normal, 0, sizeof(GLfloat)* 3);
		glVertexAttribFormat(VNORMAL, 3, GL_FLOAT, GL_FALSE, 0);
		glEnableVertexAttribArray(VNORMAL);
	}

#pragma endregion

#pragma region setup material uniform block
	{
		//initialize material 
		float spower = 33;
		glm::vec3 ambient(0.2, 0.2, 0.2);
		glm::vec3 diffuse(0.4, 0.4, 0.4);
		glm::vec3 specular(0.8, 0.8, 0.8);
		material = Material(ambient, diffuse, specular, spower);

		//create, bind and initialize material ubo (bind to binding position 0 of the uniform buffer bindings)
		glGenBuffers(1, &uboMaterial);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboMaterial);
		glBufferStorage(GL_UNIFORM_BUFFER, sizeof(material), &material, GL_MAP_WRITE_BIT);
	}
#pragma endregion

#pragma region setup light uniform block
	{
		//initialize material 
		glm::vec3 pos = glm::vec3(0, 0, 100);
		glm::vec3 ambient(1, 1, 1);
		glm::vec3 diffuse(1, 1, 1);
		glm::vec3 specular(1, 1, 1);
		light = PointLight(pos, ambient, diffuse, specular);

		//create, bind and initialize material ubo (bind to binding position 1 of the uniform buffer bindings)
		glGenBuffers(1, &uboLight);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboLight);
		glBufferStorage(GL_UNIFORM_BUFFER, sizeof(light), &light, GL_MAP_WRITE_BIT);
	}
#pragma endregion

	//by now everything is recorded in the vao
	//so we will unbind the buffer
	//and unbind the vao
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

bool PointCloud::updateObject(double time)
{
	render();
	return true;
}

bool PointCloud::destroyObject()
{
	return true;
}

static glm::mat4 rotation;

bool PointCloud::render()
{
	//render
	glUseProgram(program);
	glBindVertexArray(vao);
	/*do not transpose here, because glm has gracefully handled it.*/

	scene->camera->getWorld2View();

	//add some rotation for fun
	rotation = glm::rotate(glm::radians(270.f), glm::vec3(1, 0, 0));

	//update uniforms
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboMaterial);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboLight);
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(rotation));
	glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(scene->camera->getWorld2View()));
	glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(scene->camera->getPersProj()));
	
	glPointSize(5);
	glDrawArrays(GL_POINTS, 0, pointNum);

	//unbind vao/shader program
	glBindVertexArray(0);
	glUseProgram(0);

	return true;
}