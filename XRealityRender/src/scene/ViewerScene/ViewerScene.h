#ifndef VIEWERSCENE_H
#define VIEWERSCENE_H

#include <GLFW\glfw3.h>
#include "..\..\imgui\imgui.h"
#include "..\..\XREntity.h"

#include "ViewerSceneGUI.h"
#include "..\..\entity\RotatingTriangle.h"

class ViewerScene :public XREntity
{
public:
	virtual bool init();
	virtual bool update(float time);
	virtual bool destroy();
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
private:
	RotatingTriangle *triangle;
	ViewerSceneGUI *gui;
};


#endif