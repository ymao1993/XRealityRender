#ifndef FORESTSCENE_H
#define FORESTSCENE_H

#include "../../XRGLFW.h"

#include "../../XRScene.h"
#include "../../XRObject.h"
#include "../../XRCamera.h"

class ForestScene :public XRScene
{
public:
	virtual bool initScene();
	virtual bool updateScene(double time);
	virtual bool destroyScene();
	virtual XRUserInterface* createUserInterface();
};


#endif