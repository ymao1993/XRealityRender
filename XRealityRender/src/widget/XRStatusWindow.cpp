#include "../XRCommon.h"
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
	
	//fps and delta time
	ImGui::TextColored(ImVec4(0, 1, 1, 1), "FPS: %.5lf", 1. / time);
	ImGui::TextColored(ImVec4(0, 1, 1, 1), "Delta Time: %.5lf", time);

	//opengl version
	int major = 0;
	int minor = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	ImGui::TextColored(ImVec4(1, 0, 0, 1), "OpenGL: %d.%d", major, minor);

	//hardware vendor
	const char *sVendor = (char*)glGetString(GL_VENDOR);
	ImGui::TextColored(ImVec4(1, 0, 0, 1), "Vendor: %s", sVendor);


	ImGui::End();
	return true;
}

bool XRStatusWindow::destroy()
{
	return true;
}