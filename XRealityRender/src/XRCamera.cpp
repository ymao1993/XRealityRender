#include "XRCamera.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\rotate_vector.hpp>


bool XRCamera::init()
{
	//TODO: make these variables configurable!
	aspect = XREngine::instance()->getWindowW() / XREngine::instance()->getWindowW();
	zNear = 0.01f;
	zFar = 100.f;
	fovy = 45.f;
	speed = 0.03f;
	rotateMaxSpeed = 0.01f;

	//initialize position& orientation of the camera
	position = glm::vec3(0, 0, 10);
	up = glm::vec3(0,1,0);
	front = glm::vec3(0, 0, -1);

	return true;
}

bool XRCamera::update(double time)
{
	//update aspect
	//change it in the future??
	aspect = XREngine::instance()->getWindowW() / XREngine::instance()->getWindowW();

	updatePosition();
	updateOrientation();
	return true;
}

void XRCamera::updatePosition()
{
	if (XRDevice::isKeyPressed(GLFW_KEY_W))
	{
		position += glm::normalize(front) * speed;
	}
	if (XRDevice::isKeyPressed(GLFW_KEY_S))
	{
		position -= glm::normalize(front) * speed;
	}
	if (XRDevice::isKeyPressed(GLFW_KEY_A))
	{
		position -= glm::normalize(glm::cross(front, up)) * speed;
	}
	if (XRDevice::isKeyPressed(GLFW_KEY_D))
	{
		position += glm::normalize(glm::cross(front, up)) * speed;
	}
}

bool XRCamera::destroy()
{
	return true;
}

glm::mat4 XRCamera::getWorld2Clip()
{
	glm::mat4 matPerspProj = getPersProj();
	glm::mat4 matWorld2View = getWorld2View();;
	return matPerspProj * matWorld2View;
}

glm::mat4 XRCamera::getWorld2View()
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 XRCamera::getPersProj()
{
	return glm::perspective(fovy, aspect, zNear, zFar);
}

void XRCamera::updateOrientation()
{
	glm::vec2 offset = XRDevice::getMouseSpeed();			//TODO: divide by delta time to get mouse speed
	offset.x = -offset.x / XREngine::instance()->getWindowW();
	offset.y = -offset.y / XREngine::instance()->getWindowH();
	offset *= rotateMaxSpeed;

	front = glm::rotateX(front, offset.y);
	front = glm::rotateY(front, offset.x);
}