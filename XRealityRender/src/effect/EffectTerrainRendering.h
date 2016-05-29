#ifndef TERRAINRENDERING_H
#define TERRAINRENDERING_H

#include "../XREffect.h"

class TerrainRendering :public XREffect
{
public:
	//extended functions
	virtual bool initEffect();
	virtual bool updateEffect(double time);
	virtual bool destroyEffect();

private:
	GLuint vbo_pos;
	GLuint vbo_idx;
	GLuint vbo_txc;
};



#endif