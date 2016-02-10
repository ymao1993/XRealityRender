#include "..\gl3w\gl3w.h"
#include "TestTriangle.h"
#include "../utils/XRShaderUtils.h"
#include <glm/gtc/type_ptr.hpp>

bool TestTriangle::init()
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
	const GLfloat positions[20] = { 0.0, 0.0, 0.0, 1.0,
								    0.5, 0.0, 0.0, 1.0,
								    0.0, 0.5, 0.0, 1.0,
								    0.3, 0.5, 0.0, 1.0,
								    0.5, 0.5, 0.0, 1.0 };
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_MAP_WRITE_BIT);

	//bind vbo to attribute 0 by binding index 0
	enum{VPOS};
	glVertexAttribBinding(VPOS, 0);
	glBindVertexBuffer(0, vbo, 0, sizeof(GLfloat)* 4);
	glVertexAttribFormat(VPOS, 4, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(VPOS);

	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	return true;
}

bool TestTriangle::update(double time)
{
	return true;
}

bool TestTriangle::destroy()
{
	return true;
}

bool TestTriangle::render()
{
	//render
	glUseProgram(program);
	glBindVertexArray(vao);
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(transform));
	glDrawArrays(GL_POINTS, 0, 5);


	//unbind vao/shader program
	glBindVertexArray(0);
	glUseProgram(0);

	return true;
}