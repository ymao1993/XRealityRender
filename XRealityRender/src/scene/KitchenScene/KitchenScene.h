#ifndef KITCHENSCENE_H
#define KITCHENSCENE_H

#include "../../XRGLFW.h"

#include "../../XRScene.h"
#include "../../XRObject.h"
#include "../../XRCamera.h"

class KitchenScene :public XRScene
{
public:
	virtual bool initScene();
	virtual bool updateScene(double time);
	virtual bool destroyScene();
	virtual XRUserInterface* createUserInterface();
};


#endif