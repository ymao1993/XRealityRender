#ifndef XRUSERINTERFACE_H
#define XRUSERINTERFACE_H

#include "XREntity.h"

class XRScene;

/**
 * XRUserInterface
 * XRUserInterface is the base class representing the user interface of a scene.
 * An XRUserInterface may contain multiple instances of XRWidget.
 *
 * @Author Yu Mao
 */
class XRUserInterface : public XREntity
{
public:

	//Constructor
	XRUserInterface(XRScene *scene) { this->scene = scene; }

	//extended from XREntity
	virtual bool init() final;
	virtual bool update(double time) final;
	virtual bool destroy() final;

private:
	//interface
	virtual bool initUI() = 0;
	virtual bool updateUI(double time) = 0;
	virtual bool destroyUI() = 0;

private:
	XRScene *scene;
};


#endif