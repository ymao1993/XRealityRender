#include "..\..\gl3w\gl3w.h"
#include "ViewerSceneGUI.h"
#include "../../XRCommon.h"

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

	configWindow = new XRConfigWindow();
	configWindow->init();
	
	return true;

}

bool ViewerSceneGUI::updateUI(double time)
{
	debugInfoWindow->update(time);
	statusWindow->update(time);
	consoleWindow->update(time);
	configWindow->update(time);

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

	configWindow->destroy();
	delete configWindow;

	return true;
}

