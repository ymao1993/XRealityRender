#ifndef XRWIDGET_H
#define XRWIDGET_H

#include "XREntity.h"

/**
 * XRWidget
 * XRWidget provides the interface for all ui element.
 *
 * @Author Yu Mao
 */
class XRWidget :public XREntity
{
public:
	virtual bool init() = 0;
	virtual bool update(double time) = 0;
	virtual bool destroy() = 0;
};




#endif