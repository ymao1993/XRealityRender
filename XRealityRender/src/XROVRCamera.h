#ifndef XROVRCAMERA_H
#define XROVRCAMERA_H

#include "gl3w\gl3w.h"

#include "XREntity.h"
#include <glm\glm.hpp>
#include "XRScene.h"

/**
 * XRCamera
 * XRCamera simulate the behavior of camera and provide word2clip matrix.
 * The default implementation of updatePosition and updatePosition responds
 * to key event of w,s,a,d and mouse movement. To create a new camera that
 * responds to other event, please extend a new class and override these two methods.
 *
 * @Author Yu Mao
 */
class XROVRCamera : public XREntity
{
public:
	virtual bool init();
	virtual bool update(double time);
	virtual bool destroy();

public:
	/**
	 * get worldspace-to-clipspace transformation matrix
	 */
	glm::mat4 getWorld2Clip();
	glm::mat4 getWorld2View();
	glm::mat4 getPersProj();

	void setScene(XRScene* scene);

private: 
	XRScene* scene;
};

#endif