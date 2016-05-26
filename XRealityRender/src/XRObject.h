#ifndef XROBJECT_H
#define XROBJECT_H

#include "XREntity.h"
#include "XRCamera.h"
#include "glm\glm.hpp"

#include "XRTransform.h"
#include "XRMaterial.h"
#include "XREffect.h"
#include "XRMesh.h"
#include "XRTexture.h"

#include "XRComponentType.h"

#include <map>

#include <string>

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
	friend class XRSceneWindow;

public:
	std::string name;
	XRObject(std::string name) :name(name){}

private:
	/*extended from XREntity*/
	virtual bool init() final;
	virtual bool update(double time) final;
	virtual bool destroy() final;

public:
	virtual bool initObject() = 0;
	virtual bool updateObject(double time) = 0;
	virtual bool destroyObject() = 0;

	glm::mat4 model2World;

public:

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

public:
	/*scene*/
	XRScene* scene;


};

#endif