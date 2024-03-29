#ifndef XRDEVICE_H
#define XRDEVICE_H

#include <cstring>
#include "XRGLFW.h"
#include "glm\glm.hpp"

#define KEY_NUM (GLFW_KEY_LAST+1)

/**
 * XRDevice
 * XRDevice is used to manage device IO.
 *
 * @Author Yu Mao
 */
namespace XRDevice
{
	void init(GLFWwindow* window);
	void update(double deltaTime);

	//key
	bool isKeyPressed(int key);

	//mouse
	glm::vec2 getMousePos();
	glm::vec2 getMouseOffset();
	glm::vec2 getMouseSpeed();

};


#endif