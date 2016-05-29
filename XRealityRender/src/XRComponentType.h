#ifndef XRCOMPONENTTYPE_H
#define XRCOMPONENTTYPE_H

#include <map>
#include <string>

enum XRComponentType
{
	XR_COMPONENT_MESH,
	XR_COMPONENT_TEXTURE,
	XR_COMPONENT_TEXTURE_DISPLACEMENT,
	XR_COMPONENT_TEXTURE_COLOR,
	XR_COMPONENT_MATERIAL,
	XR_COMPONENT_EFFECT,
	XR_COMPONENT_SOUND,
};


const std::map<XRComponentType, std::string> XRComponentNameMap
{
	{ XR_COMPONENT_MESH, "mesh" },
	{ XR_COMPONENT_TEXTURE, "texture" },
	{ XR_COMPONENT_TEXTURE_DISPLACEMENT, "displacement texture" },
	{ XR_COMPONENT_TEXTURE_COLOR, "color texture" },
	{ XR_COMPONENT_MATERIAL, "material"},
	{ XR_COMPONENT_EFFECT, "effect"},
	{XR_COMPONENT_SOUND, "sound" },
};



#endif