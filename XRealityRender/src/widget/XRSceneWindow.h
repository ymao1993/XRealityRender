/**
 * XRSceneWindow monitors the status of the scene
 */

#ifndef XRSCENEWINDOW_H
#define XRSCENEWINDOW_H

#include "../XRScene.h"
#include "../XRWidget.h"

class XRSceneWindow : public XRWidget
{
public:
	XRSceneWindow():scene(NULL){}
	virtual bool init();
	virtual bool update(double time);
	virtual bool destroy();

public:
	void setScene(XRScene* scene) { this->scene = scene; }

private:
	XRScene *scene;

	

};


#endif