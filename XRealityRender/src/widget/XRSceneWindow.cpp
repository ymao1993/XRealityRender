#include "XRSceneWindow.h"
#include "../XRDebug.h"
#include "../XRIMUI.h"

#include "../XRComponentType.h"

bool XRSceneWindow::init()
{
	return true;
}

bool XRSceneWindow::update(double time)
{
	ImGui::Begin("Scene");

	for (int i = 0; i < scene->objects.size(); i++)
	{
		XRObject* object = scene->objects[i];
		if (ImGui::TreeNode((void*)(intptr_t)i, object->name.c_str()))
		{
			std::map<XRComponentType, XRComponent*>::iterator iter;
			int j;
			for (iter = object->components.begin(), j = 0; iter != object->components.end(); iter++, j++)
			{
				XRComponentType type = iter->first;
				if (ImGui::TreeNode((void*)(intptr_t)j, XRComponentNameMap.find(type)->second.c_str()))
				{
					//further info
					ImGui::Text(iter->second->getDescription().c_str());
					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
	}

	ImGui::End();

	return true;
}

bool XRSceneWindow::destroy()
{
	return true;
}