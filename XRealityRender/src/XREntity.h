#ifndef XRENTITY_H
#define XRENTITY_H

/**
 * XREntity
 * XREntity is an game entity in a general sense. It can be GameObject, UI, effects, etc.
 * 
 * @Author Yu Mao
 */
class XREntity
{
public:
	virtual bool init();
	virtual bool update(double time);
	virtual bool destroy();
};


#endif