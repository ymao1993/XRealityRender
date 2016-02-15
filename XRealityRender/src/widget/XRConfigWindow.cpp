#include "XRConfigWindow.h"
#include "../XRDebug.h"
#include "../XRIMUI.h"

bool XRConfigWindow::init()
{
	return true;
}

bool XRConfigWindow::update(double time)
{
	// Update Slider Window
	ImGui::Begin("Configurations");

	ImGui::Spacing();
	static float wrap_width = 200.0f;
	ImGui::SliderFloat("Wrap width", &wrap_width, -20, 600, "%.0f");

	const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK" };
	static int item2 = -1;
	ImGui::Combo("combo scroll", &item2, items, IM_ARRAYSIZE(items));

	static int i1 = 50, i2 = 42;
	ImGui::DragInt("drag int", &i1, 1);
	ImGui::DragInt("drag int 0..100", &i2, 1, 0, 100, "%.0f%%");

	static float f1 = 1.00f, f2 = 0.0067f;
	ImGui::DragFloat("drag float", &f1, 0.005f);
	ImGui::DragFloat("drag small float", &f2, 0.0001f, 0.0f, 0.0f, "%.06f ns");

	// list box
	ImGui::PushItemWidth(80);
	ImGui::Text("Lists:");
	static int selection[4] = { 0, 1, 2, 3 };
	for (int i = 0; i < 4; i++)
	{
		if (i > 0) ImGui::SameLine();
		ImGui::PushID(i);
		ImGui::ListBox("", &selection[i], items, IM_ARRAYSIZE(items));
		ImGui::PopID();
		//if (ImGui::IsItemHovered()) ImGui::SetTooltip("ListBox %d hovered", i);
	}
	ImGui::PopItemWidth();
	ImGui::End();
	return true;
}

bool XRConfigWindow::destroy()
{
	return true;
}