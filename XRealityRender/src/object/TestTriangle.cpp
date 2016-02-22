#include "../gl3w/gl3w.h"
#include "TestTriangle.h"
#include "../utils/XRShaderUtils.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

//create and initialize vbo
const GLfloat positions[24] =
{
	-0.5, 0.0, 0.0, 1.0,
	0.5, 0.0, 0.0, 1.0,
	0.0, 1.0, 0.0, 1.0,
	-0.5, 0.0, -1.0, 1.0,
	0.5, 0.0, -1.0, 1.0,
	0.0, 1.0, -1.0, 1.0,

};

bool TestTriangle::initObject()
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


	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_MAP_WRITE_BIT);

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

bool TestTriangle::updateObject(double time)
{
	return true;
}

bool TestTriangle::destroyObject()
{
	return true;
}

static glm::mat4 rotation;

bool TestTriangle::render()
{
	//add some rotation for fun
	rotation = glm::rotate(rotation, glm::radians(1.f), glm::vec3(0.f, 1.f, 0.f));


	//render
	glUseProgram(program);
	glBindVertexArray(vao);
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(camera->getWorld2Clip()));
	glDrawArrays(GL_TRIANGLES, 0, 7);


	//unbind vao/shader program
	glBindVertexArray(0);
	glUseProgram(0);

	return true;
}
