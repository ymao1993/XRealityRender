#ifndef XRCOMPONENT_H
#define XRCOMPONENT_H

class XRObject;

#include "XREntity.h"

enum XRComponentType;

class XRComponent :public XREntity
{
public:
	XRComponent(XRComponentType type) :type(type){}

	/*extended from XRentity*/
	virtual bool init() = 0;
	virtual bool update(double time) = 0;
	virtual bool destroy() = 0;

protected:
	XRObject *object;  //the object containing this component

private:
	void setObject(XRObject* object) { this->object = object; }
	XRComponentType type;

	friend  class XRObject;
};

#endif