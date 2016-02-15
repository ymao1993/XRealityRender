#include "../gl3w/gl3w.h"
#include "TestTriangle.h"
#include "../utils/XRShaderUtils.h"
#include <glm/gtc/type_ptr.hpp>

bool TestTriangle::init()
{
#pragma region build shaders
	//load shaders
	GLuint shaders[2];
	shaders[0] = XRShaderUtils::loadShader("res/shader/PhongLighting(GS)/phongLighting(gs).vs.glsl", GL_VERTEX_SHADER);
	shaders[1] = XRShaderUtils::loadShader("res/shader/PhongLighting(GS)/phongLighting(gs).fs.glsl", GL_FRAGMENT_SHADER);
	//link program
	program = XRShaderUtils::linkShaderProgram(shaders, 2, true);

#pragma endregion

	//create and initialize vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Vertex Attributes
	enum{ VPOS, VNORMAL };

#pragma region setup position attribute
	//create and initialize position vbo_pos
	const GLfloat positions[12] = { -1.0, 0.0, 0.0, 1.0,
								    1, 0.0, 0.0, 1.0,
								    0.0, 2, 0.0, 1.0 };
	GLuint vbo_pos;
	glGenBuffers(1, &vbo_pos);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_MAP_WRITE_BIT);

	//binding vertex attribute with vertex buffer object
	glVertexAttribBinding(VPOS, 0);
	glBindVertexBuffer(0, vbo_pos, 0, sizeof(GLfloat)* 4);
	glVertexAttribFormat(VPOS, 4, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(VPOS);

#pragma endregion

#pragma region setup normal attribute
	//create and initialize normals vbo
	const GLfloat normals[9] = { 0.0, 0.0, 1.0,
								 0.0, 0.0, 1.0,
								 0.0, 0.0, 1.0,};
	GLuint vbo_normal;
	glGenBuffers(1, &vbo_normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_MAP_WRITE_BIT);

	//binding vertex attribute with vertex buffer object
	glVertexAttribBinding(VNORMAL, 1);
	glBindVertexBuffer(1, vbo_normal, 0, sizeof(GLfloat)* 3);
	glVertexAttribFormat(VNORMAL, 3, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(VNORMAL);

#pragma endregion

	//by now everything is recorded in the vao
	//so we will unbind the buffer
	//and unbind the vao
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
	glUseProgram(program);
	glBindVertexArray(vao);
	/*do not transpose here, because glm has gracefully handled it.*/
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(camera->getWorld2View())); 
	glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(camera->getPersProj()));

	glDrawArrays(GL_TRIANGLES, 0, 3);

	//unbind vao/shader program
	glBindVertexArray(0);
	glUseProgram(0);

	return true;
}