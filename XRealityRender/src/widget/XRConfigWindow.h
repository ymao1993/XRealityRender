#ifndef XRCONFIGWINDOW_H
#define XRCONFIGWINDOW_H
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

#include "../XRWidget.h"

class XRConfigWindow : public XRWidget
{
public:
	virtual bool init();
	virtual bool update(double time);
	virtual bool destroy();
};


#endif