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


	return true;

}

bool ViewerSceneGUI::updateUI(double time)
{
	debugInfoWindow->update(time);
	statusWindow->update(time);

	return true;
}

bool ViewerSceneGUI::destroyUI()
{
	debugInfoWindow->destroy();
	delete debugInfoWindow;

	statusWindow->destroy();
	delete statusWindow;

	return true;
}

