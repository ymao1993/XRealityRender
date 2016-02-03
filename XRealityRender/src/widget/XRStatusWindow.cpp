#include "XRStatusWindow.h"
#include "../XRDebug.h"
#include "../XRIMUI.h"

bool XRStatusWindow::init()
{
	return true;
}

bool XRStatusWindow::update(double time)
{
	//Update Debugging Window
	ImGui::Begin("Status");
	ImGui::Text("FPS: %.5lf", 1./time);
	ImGui::Text("Delta Time: %.5lf", time);
	ImGui::End();
	return true;
}

bool XRStatusWindow::destroy()
{
	return true;
}