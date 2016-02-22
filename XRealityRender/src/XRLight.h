#ifndef XRLIGHT_H
#define XRLIGHT_H

#include "glm\glm.hpp"

enum XRLIGHT_TYPE
{
	XRLIGHT_POINT,
};

struct XRLight
{
	XRLIGHT_TYPE type;
};

struct XRPointLight: public XRLight
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 position;
};


#endif