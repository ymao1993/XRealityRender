#include "../../XRCommon.h"
#include "KitchenSceneGUI.h"

KitchenSceneGUI::KitchenSceneGUI(XRScene* scene):
XRUserInterface(scene){}

bool KitchenSceneGUI::initUI()
{
	debugInfoWindow = new XRDebugOutputWindow();
	debugInfoWindow->init();

	statusWindow = new XRStatusWindow();
	statusWindow->init();

	consoleWindow = new XRConsoleWindow();
	consoleWindow->init();

	sceneWindow = new XRSceneWindow();
	sceneWindow->setScene(scene);
	sceneWindow->init();

	shaderInspector = new XRShaderInspector();
	shaderInspector->init();
	
	return true;

}

bool KitchenSceneGUI::updateUI(double time)
{
	debugInfoWindow->update(time);
	statusWindow->update(time);
	consoleWindow->update(time);
	sceneWindow->update(time);
	shaderInspector->update(time);

	return true;
}

bool KitchenSceneGUI::destroyUI()
{
	debugInfoWindow->destroy();
	delete debugInfoWindow;

	statusWindow->destroy();
	delete statusWindow;

	consoleWindow->destroy();
	delete consoleWindow;

	sceneWindow->destroy();
	delete sceneWindow;

	shaderInspector->destroy();
	delete shaderInspector;

	return true;
}

