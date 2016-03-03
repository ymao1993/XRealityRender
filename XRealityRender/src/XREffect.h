#ifndef XREFFECT_H
#define XREFFECT_H

#include "gl3w\gl3w.h"
#include "XRComponent.h"
#include "XRComponentType.h"

/**
 * XREffect Represents a shader program.
 *
 * XREffect is responsible for fetching data from XRObject
 * where it is contained, and provide them in the right
 * format to the shader program it represents.
 *
 * Note: To create a custom rendering effect, you have to
 * create a new class extending from XREffect.
 *
 * @Author Yu Mao
 */


class XREffect :public XRComponent
{
public:

	XREffect() :XRComponent(XR_COMPONENT_EFFECT){}

	virtual bool initEffect() = 0;
	virtual bool updateEffect(double time) = 0;
	virtual bool destroyEffect() = 0;

private:
	virtual bool init();
	virtual bool update(double time);
	virtual bool destroy();

protected:
	GLuint program;
	GLuint vao;
};

#endif