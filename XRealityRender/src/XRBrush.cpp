#include "XRBrush.h"
#include "XRScene.h"
#include "utils\XRShaderUtils.h"


/*vertex attribute*/
enum
{
	VPOS
};

static void setup(const float* data, int num, GLuint& vao, GLuint& vbo);
static void cleanup(GLuint vao, GLuint vbo);


/*shader uniform*/
enum
{
	TRANSFORMATION_MATRIX,
	COLOR
};

XRBrush::XRBrush(XRScene * scene):scene(scene)
{
	//build shader program
	GLuint shaders[2];
	shaders[0] = XRShaderUtils::loadShader("res/shader/SingleColor/SingleColor.vs.glsl", GL_VERTEX_SHADER);
	shaders[1] = XRShaderUtils::loadShader("res/shader/SingleColor/SingleColor.fs.glsl", GL_FRAGMENT_SHADER);
	program = XRShaderUtils::linkShaderProgram(shaders, 2, true);

	//set default color
	color = vec4(1,0,0,1);
}

XRBrush::~XRBrush()
{
	glDeleteProgram(program);
}


/*batch rendering*/
void XRBrush::render(vec4 color)
{
	if (color == vec4(0, 0, 0, 0)) color = this->color;

	//draw all points
	if (IPoints.size())
	{
		GLuint vao, vbo;
		setup(&IPoints[0], IPoints.size(), vao, vbo);
		setUniforms();
		glDrawArrays(GL_POINTS, 0, IPoints.size());
		cleanup(vao, vbo);
	}

	if (DPoints.size())
	{
		GLuint vao, vbo;
		setup(&DPoints[0], DPoints.size(), vao, vbo);
		setUniforms();
		glDrawArrays(GL_POINTS, 0, DPoints.size());
		cleanup(vao, vbo);
	}

	//draw all lines
	if (ILines.size())
	{
		GLuint vao, vbo;
		setup(&ILines[0], ILines.size(), vao, vbo);
		setUniforms();
		glDrawArrays(GL_LINES, 0, ILines.size());
		cleanup(vao, vbo);
	}

	if (DLines.size())
	{
		GLuint vao, vbo;
		setup(&DLines[0], DLines.size(), vao, vbo);
		setUniforms();
		glDrawArrays(GL_LINES, 0, DLines.size());
		cleanup(vao, vbo);
	}

	//draw all triangles
	GLint previous[2];
	glGetIntegerv(GL_POLYGON_MODE, previous);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (ITriangles.size())
	{
		GLuint vao, vbo;
		setup(&ITriangles[0], ITriangles.size(), vao, vbo);
		setUniforms();
		glDrawArrays(GL_TRIANGLES, 0, ITriangles.size());
		cleanup(vao, vbo);
	}

	if (DTriangles.size())
	{
		GLuint vao, vbo;
		setup(&DTriangles[0], DTriangles.size(), vao, vbo);
		setUniforms();
		glDrawArrays(GL_TRIANGLES, 0, DTriangles.size());
		cleanup(vao, vbo);
	}
	glPolygonMode(GL_FRONT, previous[0]);
	glPolygonMode(GL_BACK, previous[1]);

}

void XRBrush::Iclear()
{
	IPoints.clear();
	ILines.clear();
	ITriangles.clear();
}

void XRBrush::Dclear()
{
	DPoints.clear();
	DLines.clear();
	DTriangles.clear();
}

void XRBrush::drawTriangle(vec3 p1, vec3 p2, vec3 p3, bool immediate)
{
	if (immediate)
	{
		ITriangles.push_back(p1.x);	ITriangles.push_back(p1.y);	ITriangles.push_back(p1.z);
		ITriangles.push_back(p2.x);	ITriangles.push_back(p2.y);	ITriangles.push_back(p2.z);
		ITriangles.push_back(p3.x);	ITriangles.push_back(p3.y);	ITriangles.push_back(p3.z);
	}
	else
	{
		DTriangles.push_back(p1.x);	DTriangles.push_back(p1.y);	DTriangles.push_back(p1.z);
		DTriangles.push_back(p2.x);	DTriangles.push_back(p2.y);	DTriangles.push_back(p2.z);
		DTriangles.push_back(p3.x);	DTriangles.push_back(p3.y);	DTriangles.push_back(p3.z);
	}
	return;
}

void XRBrush::drawLine(vec3 p1, vec3 p2, bool immediate)
{
	if (immediate)
	{
		ILines.push_back(p1.x);	ILines.push_back(p1.y);	ILines.push_back(p1.z);
		ILines.push_back(p2.x);	ILines.push_back(p2.y);	ILines.push_back(p2.z);
	}
	else
	{
		DLines.push_back(p1.x);	DLines.push_back(p1.y);	DLines.push_back(p1.z);
		DLines.push_back(p2.x);	DLines.push_back(p2.y);	DLines.push_back(p2.z);
	}
	return;
}

void XRBrush::drawPoint(vec3 p, bool immediate)
{
	if (immediate)
	{
		IPoints.push_back(p.x);	IPoints.push_back(p.y);	IPoints.push_back(p.z);
	}
	else
	{
		DPoints.push_back(p.x);	DPoints.push_back(p.y);	DPoints.push_back(p.z);
	}
	return;
}

void XRBrush::drawQuadrilateral(vec3 p1, vec3 p2, vec3 p3, vec3 p4, bool immediate)
{
	drawLine(p1, p2, immediate);
	drawLine(p2, p3, immediate);
	drawLine(p3, p4, immediate);
	drawLine(p4, p1, immediate);

	return;
}

void XRBrush::drawBox(vec3 p1, vec3 p2, vec3 p3, vec3 p4, vec3 p5, vec3 p6, vec3 p7, vec3 p8, bool immediate)
{
	drawQuadrilateral(p1, p2, p3, p4, immediate);
	drawQuadrilateral(p5, p6, p7, p8, immediate);
	drawLine(p1, p5, immediate);
	drawLine(p2, p6, immediate);
	drawLine(p3, p7, immediate);
	drawLine(p4, p8, immediate);

	return;
}

void XRBrush::drawBox(vec3 p0, float xspan, float yspan, float zspan, bool immediate)
{
	drawBox(p0, 
			vec3(p0.x + xspan, p0.y, p0.z), 
			vec3(p0.x + xspan, p0.y + yspan, p0.z), 
			vec3(p0.x, p0.y + yspan, p0.z),
		    vec3(p0.x, p0.y, p0.z + zspan), 
			vec3(p0.x + xspan, p0.y, p0.z + zspan), 
			vec3(p0.x + xspan, p0.y + yspan, p0.z + zspan), 
			vec3(p0.x, p0.y + yspan, p0.z + zspan), 
			immediate);

	return;
}


static void setup(const float* data, int num, GLuint& vao, GLuint& vbo)
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* num, data, GL_STREAM_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glVertexAttribBinding(VPOS, 0);
	glBindVertexBuffer(0, vbo, 0, sizeof(GLfloat)* 3);
	glVertexAttribFormat(VPOS, 3, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexAttribArray(VPOS);
}

void XRBrush::setUniforms()
{
	glUseProgram(program);
	glUniform4f(COLOR, color.r, color.g, color.b, color.a);
	glUniformMatrix4fv(TRANSFORMATION_MATRIX, 1, GL_FALSE, glm::value_ptr(scene->camera->getWorld2Clip()));
}

static void cleanup(GLuint vao, GLuint vbo)
{
	//clean-up
	glBindVertexArray(0);
	glUseProgram(0);

	//delete
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	return;
}