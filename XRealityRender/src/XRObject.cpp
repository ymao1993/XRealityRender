#include "XRObject.h"


bool XRObject::init()
{ 
	initObject();

	for (std::map<XRComponentType, XRComponent*>::iterator iter = components.begin(); iter != components.end(); iter++)
	{
		iter->second->init();
	}

	return true; 
}

bool XRObject::update(double time)
{
	updateObject(time);

	for (std::map<XRComponentType, XRComponent*>::iterator iter = components.begin(); iter != components.end(); iter++)
	{
		iter->second->update(time);
	}
	return true;
}

bool XRObject::destroy()
{
	destroyObject();

	deleteAllComponents();
	return true; 
}

/*Component Management Functions*/

XRComponent* XRObject::getComponent(XRComponentType type)
{
	if (components.find(type) != components.end())
	{
		return components[type];
	}
	return NULL;
}

void XRObject::addComponent(XRComponent* component)
{
	if (component)
	{
		component->setObject(this);
		XRComponentType type = component->type;
		components[type] = component;
	}
}

void XRObject::deleteComponent(XRComponent* component)
{
	if (component)
	{
		for (std::map<XRComponentType, XRComponent*>::iterator iter = components.begin(); iter != components.end(); iter++)
		{
			if (iter->second == component)
			{
				iter->second->destroy();
				delete iter->second;
				break;
			}
		}
	}
}

void XRObject::deleteComponent(XRComponentType type)
{
	if (components.find(type) != components.end())
	{
		components[type]->destroy();
		delete components[type];
		components.erase(type);
	}
}

void XRObject::deleteAllComponents()
{
	std::map<XRComponentType, XRComponent*>::iterator iter;
	for (iter = components.begin(); iter != components.end(); iter++)
	{
		iter->second->destroy();
		delete iter->second;
	}
	components.clear();
}