#ifndef EFFECTTEXTUREMAPPINGINSTANCED_H
#define EFFECTTEXTUREMAPPINGINSTANCED_H

#include "../XRCommon.h"
#include "../XREffect.h"

class EffectTextureMappingInstanced :public XREffect
{
public:
	EffectTextureMappingInstanced(vector<vec3> instanced_positions) :instanced_positions(instanced_positions){}

	//extended functions
	virtual bool initEffect();
	virtual bool updateEffect(double time);
	virtual bool destroyEffect();

private:
	GLuint vbo_pos;
	GLuint vbo_instanced_pos;
	GLuint vbo_idx;
	GLuint vbo_txcoords;

	vector<vec3> instanced_positions;
	
};



#endif