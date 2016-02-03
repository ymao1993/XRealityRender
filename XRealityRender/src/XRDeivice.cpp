#include "gl3w\gl3w.h"
#include "XRDevice.h"
#include "glm\glm.hpp"
#include "XREngine.h"

namespace XRDevice
{
	/*Key*/

	static bool keyStates[KEY_NUM] = { false };

	void callbackGLFW(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_RELEASE)
		{
			keyStates[key] = false;
		}
		else if (action == GLFW_PRESS)
		{
			keyStates[key] = true;
		}
	}

	bool isKeyPressed(int key)
	{
		if (key >= KEY_NUM) return false;
		return XRDevice::keyStates[key];
	}


	/*Mouse*/

	static glm::dvec2 preCursorPos;
	static glm::dvec2 curCursorPos;
	static glm::dvec2 curCursorSpeed;
	static glm::dvec2 curCursorOffset;
	static bool firstMousePos = true;

	glm::vec2 getMousePos()
	{
		return curCursorPos;
	}

	glm::vec2 getMouseOffset()
	{
		return curCursorOffset;
	}

	glm::vec2 getMouseSpeed()
	{
		return curCursorSpeed;
	}

	static void updateCursorInfo(double deltaTime)
	{
		//update mouse position
		preCursorPos = curCursorPos;
		double xpos, ypos;
		glfwGetCursorPos(XREngine::instance()->getWindow(), &xpos, &ypos);
		curCursorPos.x = xpos;
		curCursorPos.y = ypos;

		//handle the special case when the cursor info is updated for the first time
		if (firstMousePos) preCursorPos = curCursorPos;
		firstMousePos = false;

		//update offset&speed
		curCursorOffset = curCursorPos - preCursorPos;
		curCursorSpeed = curCursorOffset / deltaTime;
	}

	//initialize device module
	void init()
	{
		glfwSetCursorPos(XREngine::instance()->getWindow(),
						 XREngine::instance()->getWindowW()/2,
						 XREngine::instance()->getWindowH()/2);

		glfwSetInputMode(XREngine::instance()->getWindow(),
						 GLFW_CURSOR,
						 GLFW_CURSOR_DISABLED);
	}

	//update device information
	void update(double deltaTime)
	{
		updateCursorInfo(deltaTime);
	}

};