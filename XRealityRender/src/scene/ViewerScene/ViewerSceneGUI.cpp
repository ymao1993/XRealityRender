#include "../../XRCommon.h"
#include "ViewerSceneGUI.h"

ViewerSceneGUI::ViewerSceneGUI(XRScene* scene):
XRUserInterface(scene){}

bool ViewerSceneGUI::initUI()
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
	
	return true;

}

bool ViewerSceneGUI::updateUI(double time)
{
	debugInfoWindow->update(time);
	statusWindow->update(time);
	consoleWindow->update(time);
	sceneWindow->update(time);

	return true;
}

bool ViewerSceneGUI::destroyUI()
{
	debugInfoWindow->destroy();
	delete debugInfoWindow;

	statusWindow->destroy();
	delete statusWindow;

	consoleWindow->destroy();
	delete consoleWindow;

	sceneWindow->destroy();
	delete sceneWindow;

	return true;
}

