#include "GObject.h"
#include "tool/Tool.hpp"

USING_NS_CC;

const float GObject::ROT_MIN = 0.1f;
const float GObject::MOV_MIN = 0.1f;
const float GObject::ACC_MIN = 0.1f;

void GObject::setVelocity(const cocos2d::Vec2 & vec)
{
	m_Velocity = vec;
}

void GObject::setAcceleration(const cocos2d::Vec2 & ac)
{
	m_Acceleration = ac;
}

void GObject::setCamera(GCamera *_camera)
{
	m_Camera = _camera;
}

GCamera* GObject::getCamera()
{
	return m_Camera;
}

void GObject::stopMoveAndRotate()
{
	m_AgSpeed = 0;
	stopMove();
}

void GObject::stopMove()
{
	m_Velocity = { 0, 0 };
	m_Acceleration = { 0, 0 };
}
void GObject::setAgSpeed(float ag)
{
	m_AgSpeed = ag;
}

cocos2d::Vec2 GObject::getVelocity() const
{
	return m_Velocity;
}

cocos2d::Vec2 GObject::getAcceleration() const
{
	return m_Acceleration;
}

float GObject::getAgSpeed() const
{
	return m_AgSpeed;
}

Animation* GObject::getAnimation(int type, float fps)
{
	Vector<SpriteFrame*>& frames = m_Animations[type];
	Animation* animation;
	if (fps > 0) {
		animation = Animation::createWithSpriteFrames(
			frames, 1 / fps);
	}
	else {
		animation = Animation::createWithSpriteFrames(
			frames, 1 / m_AnimFPS[type]);
	}
	return animation;
}

void GObject::setAnim(int type, const std::vector<std::string>& anims, float fps)
{
	Vector<SpriteFrame*> &tmp = m_Animations[type] = Vector<SpriteFrame*>();
	m_AnimFPS[type] = fps;
	for (const std::string &frameName : anims) {
		tmp.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName));
	}
}

void GObject::setAnim2(int type, char *format, int numFrame, int start, int count, float fps)
{
		char filename[100];
		Vector<SpriteFrame*> &tmp = m_Animations[type] = Vector<SpriteFrame*>();
		m_AnimFPS[type] = fps;
		for (int i = start; i < start + count; i++)
		{
			sprintf(filename, format, numFrame, i);
			tmp.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(filename));
		}
}

GObject::State GObject::getState()
{
	return m_CurrentState;
}

void GObject::update(float duration)
{
	if (m_AgSpeed < -ROT_MIN || m_AgSpeed > ROT_MIN) {
		setRotation(-m_AgSpeed * m_CurrentTime);
	}
	if (mtl::isVecOutRange(-MOV_MIN, MOV_MIN, m_Velocity) ||
		mtl::isVecOutRange(-ACC_MIN, ACC_MIN, m_Acceleration)) {
		m_Velocity += duration * m_Acceleration;
		setPosition(_position + m_Velocity * duration);
	}
	m_CurrentTime += duration;
}
 