#ifndef XRENGINE_H
#define XRENGINE_H

#include "XRCommon.h"
#include "XRGLFW.h"
#include <vector>

class XRScene;

/**
 * XREngine
 * XREngine controls the gaem loop, resource initialization, initial scene creation, etc.
 *
 * @Author Yu Mao
 */
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
	inline void setScene(XRScene* scene) { this->scene = scene; }
	inline XRScene* getScene(){return this->scene; }

private:
	XREngine();
	XREngine& operator= (XREngine&) {};
	XREngine(const XREngine&){};

	inline void setGLFWwindow(GLFWwindow* window) { this->window = window; }

	static XREngine* engine;
	XRScene* scene;

	//game loop
	double deltaTime;
	double secondsPerFrame;

	//windowinfo
	GLFWwindow* window;
	int windowW;
	int windowH;

	void handleKeyEvent();
};

#endif