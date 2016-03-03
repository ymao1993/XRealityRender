#ifndef XROBJECT_H
#define XROBJECT_H

#include "XREntity.h"
#include "XRCamera.h"
#include "glm\glm.hpp"

#include "XRTransform.h"
#include "XRMaterial.h"
#include "XREffect.h"
#include "XRMesh.h"

#include "XRComponentType.h";

#include <map>

/**
* XRObject
* XRObject is a visible object in the scene
*
* @Author Yu Mao
*/
class XRObject:public XREntity 
{

	//XRScene is friend to XRObject because it manages object
	friend class XRScene;

private:
	/*extended from XREntity*/
	virtual bool init() final;
	virtual bool update(double time) final;
	virtual bool destroy() final;

public:
	virtual bool initObject() = 0;
	virtual bool updateObject(double time) = 0;
	virtual bool destroyObject() = 0;

	inline void setCamera(XRCamera *camera){ this->camera = camera; }

	glm::mat4 model2World;

public:

	/*camera*/
	XRCamera *camera;

	/*transform info*/
	XRTransform transform;

	/*component management*/
	XRComponent* getComponent(XRComponentType type);
	void addComponent(XRComponent* component);
	void deleteComponent(XRComponent* component);
	void deleteComponent(XRComponentType type);
	void deleteAllComponents();

private:

	/*components*/
	std::map<XRComponentType, XRComponent*> components;


};

#endif