#include "XRCommon.h"
#include "XRUserInterface.h"
#include "XRIMUI.h"
#include "XREngine.h"

bool XRUserInterface::init()
{
	//init IMGUI
	ImGui_ImplGlfw_Init(XREngine::instance()->getWindow(), true);

	//init ui element
	initUI();
	
	return true;
}

bool XRUserInterface::update(double time)
{
	//start IMGUI new frame
	ImGui_ImplGlfw_NewFrame();

	//update UI element
	updateUI(time);

	//IMGUI draw call
	ImGui::Render();
	
	return true;
}

bool XRUserInterface::destroy()
{
	destroyUI();
	ImGui::Shutdown();
	return true;
}

