#ifndef EFFECTENVMAPPINGREFLECTREFRACT_H
#define EFFECTENVMAPPINGREFLECTREFRACT_H

#include "../XREffect.h"

class EffectEnvMappingReflectRefract :public XREffect
{
public:
	EffectEnvMappingReflectRefract(int option = 0) : option(option) {}

	//extended functions
	virtual bool initEffect();
	virtual bool updateEffect(double time);
	virtual bool destroyEffect();

private:
	GLuint vbo_pos;
	GLuint vbo_idx;
	GLuint vbo_normal;

	//option = 0: reflection
	//option = 1: refraction
	int option;
};



#endif