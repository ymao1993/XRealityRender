#ifndef EFFECTTEXTUREMAPPING_H
#define EFFECTTEXTUREMAPPING_H

#include "../XREffect.h"

class EffectTextureMapping :public XREffect
{
public:
	//extended functions
	virtual bool initEffect();
	virtual bool updateEffect(double time);
	virtual bool destroyEffect();

private:
	GLuint vbo_pos;
	GLuint vbo_idx;
	GLuint vbo_txcoords;
};



#endif