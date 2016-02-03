#ifndef XRDEBUGOUTPUTWINDOW_H
#define XRDEBUGOUTPUTWINDOW_H

#include "../XRWidget.h"

class XRDebugOutputWindow: public XRWidget
{
public:
	virtual bool init();
	virtual bool update(double time);
	virtual bool destroy();
};


#endif