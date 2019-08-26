#include "GObject3D.hpp"
#include "GAnimation3DCache.hpp"

USING_NS_CC;

const float GObject3D::ROT_MIN = 0.1f;
const float GObject3D::MOV_MIN = 0.1f;
const float GObject3D::ACC_MIN = 0.1f;

GObject3D::GObject3D() : Sprite3D() {

}

void GObject3D::setVelocity(const cocos2d::Vec3 & vec)
{
	m_Velocity = vec;
}

void GObject3D::setAcceleration(const cocos2d::Vec3 & ac)
{
	m_Acceleration = ac;
}

cocos2d::Vec3 GObject3D::getVelocity() const
{
	return m_Velocity;
}

cocos2d::Vec3 GObject3D::getAcceleration() const
{
	return m_Acceleration;
}

void GObject3D::setAgSpeed(float ag) {
	m_AgSpeed = ag;
}

void GObject3D::setRotationAxis(const cocos2d::Vec3& axis)
{
	m_RotAxis = axis;
}

void GObject3D::setAnim(int key, const char* animName, float speed)
{
	m_Animations[key] = GAnimation3DCache::getInstance()->getAnimation(animName);
}

void GObject3D::stopMoveAndRotation()
{
	m_AgSpeed = 0;
	stopMove();
}

void GObject3D::stopMove()
{
	m_Velocity = { 0, 0 };
	m_Acceleration = { 0, 0 };
}

GObject3D::State GObject3D::getState()
{
	return m_CurrentState;
}
