#ifndef XRSHADERINSPECTOR_H
#define XRSHADERINSPECTOR_H

#include "../XRWidget.h"

class XRShaderInspector : public XRWidget
{
public:
	virtual bool init();
	virtual bool update(double time);
	virtual bool destroy();
};


#endif