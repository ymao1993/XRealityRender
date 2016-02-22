#ifndef EFFECTPHONGLIGHTINGGS_H
#define EFFECTPHONGLIGHTINGGS_H

#include "../XREffect.h"

class EffectPhongLightingGS:public XREffect
{
public:
	//extended functions
	virtual bool initEffect();
	virtual bool updateEffect(double time);
	virtual bool destroyEffect();

private:
	GLuint uboMaterial;
	GLuint uboLight;
	GLuint vbo_pos;
	GLuint vbo_normal;
};



#endif