#include "XRCamera.h"
#include "XRSoundManager.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include "XREngine.h"
#include "utils\misc.h"

bool XRCamera::init()
{
	//TODO: make these variables configurable!
	aspect = (float)XREngine::instance()->getWindowW() / XREngine::instance()->getWindowH();
	zNear = 0.01f;
	zFar = 100.f;
	fovy = 45.f;
	speed = 0.06f;
	rotateMaxSpeed = 0.02f;

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
	aspect = (float)XREngine::instance()->getWindowW() / XREngine::instance()->getWindowH();

	updatePosition();
	updateOrientation();

	// update listener attribute
	FMOD_VECTOR velocity;
	velocity.x = 0;
	velocity.y = 0;
	velocity.z = 0;

	//XRSoundManager::setListenerPosition(glmToFmod(position), velocity, glmToFmod(front), glmToFmod(up));     //fix later (null window->invalid mouse position->nan front.up)
	
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
	return glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
}

void XRCamera::updateOrientation()
{
	glm::vec2 offset = XRDevice::getMouseSpeed();			//TODO: divide by delta time to get mouse speed
	offset.x = -offset.x / XREngine::instance()->getWindowW();
	offset.y = -offset.y / XREngine::instance()->getWindowH();
	offset *= rotateMaxSpeed;

	front = glm::rotateX(front, offset.y);
	front = glm::rotateY(front, offset.x);
	up = glm::rotateX(up, offset.y);
	up = glm::rotateY(up, offset.x);
}