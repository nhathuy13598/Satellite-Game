#include "SmallPoint.h"
#include "cocos2d.h"
#include "tool\Tool.hpp"
#include "GameScene.h"
USING_NS_CC;



SmallPoint * SmallPoint::createNew()
{
	return SmallPoint::create();
}

//must be called after every settings
void SmallPoint::reInit(int _Anim)
{
	stopAllActions();
	removeAllChildren();
	setBlendFunc({ GL_SRC_ALPHA, GL_ONE });
	setAgSpeed(0);
}

void SmallPoint::work()
{
	m_CurrentState = State::WORK;
	setVisible(true);
}

void SmallPoint::pause()
{
	m_CurrentState = State::PAUSE;
}

void SmallPoint::stop()
{
	m_CurrentState = State::STOP;
	setVisible(false);
}

void SmallPoint::addParticle(const char *format)
{
	auto particleX = ParticleSystemQuad::create(format);
	particleX->setVisible(true);
	particleX->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	//particleX->setTag(m_numStatus);
	this->addChild(particleX, 10);
	//m_numStatus++;
}

Sprite* SmallPoint::collisionEffect(Scene *_scene)
{

	return nullptr;
}

bool SmallPoint::init()
{
	if (!GObject::init()) {
		return false;
	}
	return true;
}

void SmallPoint::update(float duration)
{

	GObject::update(duration);
}

void SmallPoint::setPoint(int point)
{
	m_Point = point;
}

int SmallPoint::getPoint()
{
	return m_Point;
}

