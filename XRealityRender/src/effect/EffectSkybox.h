#ifndef EFFECTSKYBOX_H
#define EFFECTSKYBOX_H

#include "../XREffect.h"

class EffectSkybox :public XREffect
{
public:
	//extended functions
	virtual bool initEffect();
	virtual bool updateEffect(double time);
	virtual bool destroyEffect();

private:
	GLuint vbo_pos;
};



#endif