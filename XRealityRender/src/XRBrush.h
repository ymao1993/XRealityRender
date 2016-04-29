/**
* XRBrush is a utility class used
* for drawing primitives for debugging
* and visiualization purpose.
*
* NOTE:
*
* By design, every XRScene instance contains 
* a XRBrush. You can use XRBrush to draw primitives
* including:
* + triangle
* + line
* + point
* + quadrilateral
* + box
* There are two mode of choice: delay mode and immediate mode.
* In immediate mode, everything drawn will disapper in the next
* frame (unless you redraw them). In delay mode, everything drawn
* in the current frame will be kept in the next frame.
*
* @Author: Yu Mao
*/

#ifndef XRBRUSH_H
#define XRBRUSH_H

#include "XRCommon.h"

class XRScene;

class XRBrush
{
public:
	XRBrush(XRScene* scene);
	~XRBrush();
	
public:
	void drawTriangle(vec3 p1, vec3 p2, vec3 p3, bool immediate = true);
	void drawLine(vec3 p1, vec3 p2, bool immediate = true);
	void drawPoint(vec3 p, bool immediate = true);
	void drawQuadrilateral(vec3 p1, vec3 p2, vec3 p3, vec3 p4, bool immediate = true);
	void drawBox(vec3 p1, vec3 p2, vec3 p3, vec3 p4, vec3 p5, vec3 p6, vec3 p7, vec3 p8, bool immediate = true);
	void drawBox(vec3 p0, float xspan, float yspan, float zspan, bool immediate = true);

public:
	void setColor(vec4 color) { this->color = color; }

private:

	//immediate mode vertex buffer
	vector<float> IPoints;
	vector<float> ILines;
	vector<float> ITriangles;

	//Delay mode vertex buffer
	vector<float> DPoints;
	vector<float> DLines;
	vector<float> DTriangles;

private:
	vec4 color;
	XRScene* scene;

private:
	GLuint program;

private:
	friend XRScene;
	void render(vec4 color = vec4(0, 0, 0, 0));
	void Iclear();
	void Dclear();

private:
	void setUniforms();
};

#endif