#ifndef XRSTATUSWINDOW_H
#define XRSTATUSWINDOW_H

#include "../XRWidget.h"

class XRStatusWindow : public XRWidget
{
public:
	virtual bool init();
	virtual bool update(double time);
	virtual bool destroy();
};


#endif