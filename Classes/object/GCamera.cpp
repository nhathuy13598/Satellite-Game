#include "GCamera.h"
#include "tool/Tool.hpp"

const float GCamera::MOV_MIN = 0.1f;
const float GCamera::ACC_MIN = 0.1f;

USING_NS_CC;

GCamera * GCamera::createNew()
{
	return GCamera::create();
}

GCamera * GCamera::createPerspective(float degree, float ratio, float zNear, float zFar)
{
	GCamera* camera = GCamera::create();
	camera->initPerspective(degree, ratio, zNear, zFar);
	return camera;
}

GCamera * GCamera::createOrthographic(float width, float height, float zNear, float zFar)
{
	GCamera* camera = GCamera::create();
	camera->initOrthographic(width, height, zNear, zFar);
	return camera;
}


void GCamera::setVelocity(const cocos2d::Vec2 & vec)
{
	m_Velocity = vec;
}

void GCamera::setAcceleration(const cocos2d::Vec2 & ac)
{
	m_Acceleration = ac;
}

void GCamera::stopMove()
{
	m_Velocity = { 0, 0 };
	m_Acceleration = { 0, 0 };
}

cocos2d::Vec2 GCamera::worldToCamera(const cocos2d::Vec2 & pos)
{
	return pos - _position;
}

cocos2d::Vec2 GCamera::cameraToWorld(const cocos2d::Vec2 & pos)
{
	return pos + _position;
}

cocos2d::Vec2 GCamera::getVeclocity()
{
	return m_Velocity;
}

bool GCamera::init()
{
	if (!Camera::init()) {
		return false;
	}
	return true;
}

void GCamera::update(float duration)
{
	if (mtl::isVecOutRange(-MOV_MIN, MOV_MIN, m_Velocity) ||
		mtl::isVecOutRange(-ACC_MIN, ACC_MIN, m_Acceleration)) {
		m_Velocity += duration * m_Acceleration;
		setPosition(_position + m_Velocity * duration);
	}
}
