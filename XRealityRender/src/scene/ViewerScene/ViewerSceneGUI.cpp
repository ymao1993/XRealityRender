#include "ViewerSceneGUI.h"

bool ViewerSceneGUI::init()
{
	ImGui_ImplGlfw_Init(XREngine::instance()->getWindow(),true);
	return true;

}

bool ViewerSceneGUI::update(float time)
{
	ImGui_ImplGlfw_NewFrame();

	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = 1.0f / 60.0f;

	ImGui::Begin("My window");
	ImGui::Text("Hello, world.");
	ImGui::End();

	ImGui::Render();
	return true;
}

bool ViewerSceneGUI::destroy()
{
	ImGui::Shutdown();
	return true;
}