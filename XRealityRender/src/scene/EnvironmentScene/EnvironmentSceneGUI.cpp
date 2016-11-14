#include "../../XRCommon.h"
#include "EnvironmentSceneGUI.h"

EnvrionmentSceneGUI::EnvrionmentSceneGUI(XRScene* scene) :
	XRUserInterface(scene) {}

bool EnvrionmentSceneGUI::initUI()
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

bool EnvrionmentSceneGUI::updateUI(double time)
{
	debugInfoWindow->update(time);
	statusWindow->update(time);
	consoleWindow->update(time);
	sceneWindow->update(time);
	shaderInspector->update(time);

	return true;
}

bool EnvrionmentSceneGUI::destroyUI()
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

