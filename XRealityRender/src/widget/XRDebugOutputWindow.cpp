#include "XRDebugOutputWindow.h"
#include "../XRDebug.h"
#include "../XRIMUI.h"

bool XRDebugOutputWindow::init()
{
	return true;
}

bool XRDebugOutputWindow::update(double time)
{
	//Update Debugging Window
	ImGui::Begin("Log");
	ImGui::Text(XRDebug::logs2Str().c_str());
	ImGui::End();

	return true;
}

bool XRDebugOutputWindow::destroy()
{
	return true;
}