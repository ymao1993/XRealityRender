#ifndef XRSCENE_H
#define XRSCENE_H

#include "XREntity.h"
#include "XRUserInterface.h"
#include "XRLight.h"
#include "XRObject.h"
#include "XRBrush.h"

#include <list>

class XRSceneWindow;

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
	XRScene():brush(this){}
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
	XRObject* getObject(std::string name);

private:
	friend XRSceneWindow;
	std::vector<XRObject*> objects;

public:
	XRPointLight *light;
	XRCamera *camera;

public:
	XRBrush brush;


protected:
	XRUserInterface* gui;
};


#endif