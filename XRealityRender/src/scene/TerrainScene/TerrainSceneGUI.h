#ifndef TERRAINSCENEGUI_H
#define TERRAINSCENEGUI_H

#include "../../XRUserInterface.h"
#include "../../widget/XRDebugOutputWindow.h"
#include "../../widget/XRStatusWindow.h"
#include "../../widget/XRConsoleWindow.h"
#include "../../widget/XRConfigWindow.h"
#include "../../widget/XRSceneWindow.h"
#include "../../widget/XRShaderInspector.h"

class TerrainSceneGUI :public XRUserInterface
{
public:
	TerrainSceneGUI(XRScene* scene);
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