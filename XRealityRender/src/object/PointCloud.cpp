#include "../gl3w/gl3w.h"
#include "PointCloud.h"
#include "../utils/XRShaderUtils.h"
#include <glm/gtc/type_ptr.hpp>
#include "../utils/XRPointCloudUtils.h"

bool PointCloud::init()
{
	//load shaders
	GLuint shaders[2];
	shaders[0] = XRShaderUtils::loadShader("res/shader/testTriangle/simple_practice.vs.glsl", GL_VERTEX_SHADER);
	shaders[1] = XRShaderUtils::loadShader("res/shader/testTriangle/simple_practice.fs.glsl", GL_FRAGMENT_SHADER);

	//link program
	program = XRShaderUtils::linkShaderProgram(shaders, 2, true);

	//create and initialize vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//create and initialize vbo

	GLfloat *positions = NULL;
	pointNum = 0;
	XRDebug::log("start reading point cloud...");
	if ((positions = XRPointCloudUtils::readPts("res/model/bunny.pts", pointNum))!=NULL)
	{
		XRDebug::log("reading complete.");
	}
	else
	{
		return false;
	}

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferStorage(GL_ARRAY_BUFFER, pointNum * sizeof(GLfloat) * 4, positions, GL_MAP_WRITE_BIT);

	//bind vbo to attribute 0 by binding index 0
	enum{ VPOS };
	glVertexAttribBinding(VPOS, 0);
	glBindVertexBuffer(0, vbo, 0, sizeof(GLfloat)* 4);
	glVertexAttribFormat(VPOS, 4, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(VPOS);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	return true;
}

bool PointCloud::update(double time)
{
	return true;
}

bool PointCloud::destroy()
{
	return true;
}

bool PointCloud::render()
{
	//render
	glUseProgram(program);
	glBindVertexArray(vao);
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(transform));
	glPointSize(2);
	glDrawArrays(GL_POINTS, 0, pointNum);


	//unbind vao/shader program
	glBindVertexArray(0);
	glUseProgram(0);

	return true;
}