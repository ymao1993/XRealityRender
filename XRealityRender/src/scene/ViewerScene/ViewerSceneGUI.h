#ifndef VIEWERSCENEGUI_H
#define VIEWERSCENEGUI_H

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

#include <GLFW\glfw3.h>
#include <GLFW\glfw3native.h>

#include "..\..\XREntity.h"

#include "..\..\imgui\imgui.h"
#include "..\..\imgui\imgui_impl_glfw.h"

class ViewerSceneGUI :public XREntity
{
public:
	virtual bool init();
	virtual bool update(float time);
	virtual bool destroy();
	void enableCallback();
	void disableCallback();
private:
	GLFWwindow* g_window;
	bool callbackEnabled;
};

#endif