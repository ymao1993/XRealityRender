#ifndef XRCAMERA_H
#define XRCAMERA_H

#include "XRCommon.h"
#include "XREntity.h"


/**
 * XRCamera
 * XRCamera simulate the behavior of camera and provide word2clip matrix.
 * The default implementation of updatePosition and updatePosition responds
 * to key event of w,s,a,d and mouse movement. To create a new camera that
 * responds to other event, please extend a new class and override these two methods.
 *
 * @Author Yu Mao
 */
class XRCamera : public XREntity
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

private:

	virtual void updateOrientation();
	virtual void updatePosition();

private: 
	//attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	float aspect;/* aspect: defined as (width / hight) */
	float zNear;
	float zFar;
	float fovy;
	float speed;
	float rotateMaxSpeed;

};

#endif