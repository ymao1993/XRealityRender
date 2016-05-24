#ifndef KITCHENSCENEGUI_H
#define KITCHENSCENEGUI_H

#include "../../XRUserInterface.h"
#include "../../widget/XRDebugOutputWindow.h"
#include "../../widget/XRStatusWindow.h"
#include "../../widget/XRConsoleWindow.h"
#include "../../widget/XRConfigWindow.h"
#include "../../widget/XRSceneWindow.h"
#include "../../widget/XRShaderInspector.h"

class KitchenSceneGUI :public XRUserInterface
{
public:
	KitchenSceneGUI(XRScene* scene);
	virtual bool initUI();
	virtual bool updateUI(double time);
	virtual bool destroyUI();

private:
	XRDebugOutputWindow *debugInfoWindow;
	XRStatusWindow *statusWindow;
	XRConsoleWindow *consoleWindow;
	XRSceneWindow *sceneWindow;
	XRShaderInspector *shaderInspector;

};

#endif