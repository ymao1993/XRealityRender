#ifndef XRSCENE_H
#define XRSCENE_H

#include "XREntity.h"
#include "XRUserInterface.h"

/**
* XRScene
* XRScene represents scene, which manages all the elements 
* in a certain scene, including UI, game objects, resources, etc.
*
* @Author Yu Mao
*/
class XRScene:public XREntity
{
public:
	virtual bool init() final;
	virtual bool update(double time) final;
	virtual bool destroy() final;

private:
	virtual XRUserInterface* createUserInterface() = 0;
	virtual bool initScene() = 0;
	virtual bool updateScene(double time) = 0;
	virtual bool destroyScene() = 0;

private:
	XRUserInterface* ui;
};


#endif