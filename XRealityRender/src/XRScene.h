#ifndef XRSCENE_H
#define XRSCENE_H

#include "XREntity.h"
#include "XRUserInterface.h"
#include "XRLight.h"
#include "XRObject.h"

#include <list>

/**
* XRScene
* XRScene represents scene, which manages all the elements 
* in a certain scene, including UI, game objects, resources, etc.
*
* @Author Yu Mao
*/
class XRScene: public XREntity
{
public:
	virtual bool init() final;
	virtual bool update(double time) final;
	virtual bool destroy() final;

private:
	virtual XRUserInterface* createUserInterface() { return NULL; };
	virtual bool initScene() { return true; }
	virtual bool updateScene(double time) { return true; }
	virtual bool destroyScene() { return true; }

public:
	void addObject(XRObject* object);
	void deleteObject(XRObject* object);
	void deleteAllObjects();

private:
	std::list<XRObject*> objects;

public:
	XRPointLight *light;

private:
	XRUserInterface* ui;
};


#endif