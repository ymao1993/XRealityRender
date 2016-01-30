#ifndef XRENGINE_H
#define XRENGINE_H

#include <vector>
#include "GLFW\glfw3.h"

class ViewerScene;

class XREngine
{
public:
	bool init(GLFWwindow* window);
	bool update();
	bool destroy();
	static XREngine* instance();

public:
	inline void setWindowWH(int w, int h) { windowW = w; windowH = h; }
	inline int getWindowW(){ return windowW; }
	inline int getWindowH(){ return windowH; }
	inline GLFWwindow* getWindow(){ return window; }
	inline void setScene(ViewerScene* scene) { this->scene = scene; }

private:
	XREngine();
	XREngine& operator= (XREngine&) {};
	XREngine(const XREngine&){};

	inline void setGLFWwindow(GLFWwindow* window) { this->window = window; }

	static XREngine* engine;
	ViewerScene* scene;
	GLFWwindow* window;
	int windowW;
	int windowH;
};

#endif