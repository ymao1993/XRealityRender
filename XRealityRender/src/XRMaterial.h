#ifndef XRMATERIAL_H
#define XRMATERIAL_H

#include "glm\glm.hpp"
#include "XRComponent.h"
#include "XRComponentType.h"

class XRMaterial : public XRComponent
{
public:

	virtual bool init(){ return true; }
	virtual bool update(double time){ return true; }
	virtual bool destroy(){ return true; }

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float specularPower;

	XRMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float specularPower) 
		:XRComponent(XR_COMPONENT_MATERIAL), ambient(ambient), diffuse(diffuse), specular(specular), specularPower(specularPower){}
};

#endif