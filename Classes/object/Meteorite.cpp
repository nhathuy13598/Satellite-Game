#include "Meteorite.h"
#include "tool\Tool.hpp"

USING_NS_CC;

Meteorite * Meteorite::createNew()
{
	return Meteorite::create();
}

void Meteorite::reInit(int _Anim)
{
	setSpriteFrame(m_Animations[NORMAL_ANIM].front());
	runAction(RepeatForever::create(Animate::create(getAnimation(NORMAL_ANIM))));

	auto particleC_M = ParticleSystemQuad::create(RESOURCE(R_R_particle_meteo_plist));
	particleC_M->setVisible(true);
	particleC_M->setPosition({ this->getContentSize().width / 2, this->getContentSize().height / 2 });
	this->addChild(particleC_M, 10);
}

void Meteorite::work()
{
	m_CurrentState = State::WORK;
	setVisible(true);
}

void Meteorite::pause()
{
	m_CurrentState = State::PAUSE;
}

void Meteorite::stop()
{
	m_CurrentState = State::STOP;
	setVisible(false);
}
float Meteorite::getRadius()
{
	return _contentSize.width / 2;
}

bool Meteorite::rePos()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 position = getPosition();
	Vec2 cameraPos = m_Camera->getPosition();
	float radius = getRadius();
	// Cap nhat lai vi tri

	//if (position.y <= cameraPos.y - radius || position.y >= cameraPos.y + visibleSize.height*1.5 + radius) {
		int k = random(0, 1);
		if (k == 1)
			position.x = random(int(visibleSize.width * 1.5), int(visibleSize.width * 2));
		if (k == 0)
			position.x = random(int(-visibleSize.width * 1.5), int(-visibleSize.width));

		position.y = random(int(visibleSize.height * 1.5), int(visibleSize.height * 2));
		Vec2 worldPosition = m_Camera->cameraToWorld(position);
		setPosition(worldPosition);
		return true;
	//}
	return false;
}

bool Meteorite::init()
{
	if (!GObject::init()) {
		return false;
	}
	Vec2 position;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float width = visibleSize.width;
	float height = visibleSize.height;
	while (position.x < visibleSize.width*2 && position.x > -visibleSize.width)
	{
		position.x = random(int(-visibleSize.width*2), int(visibleSize.width * 2));
	}
	position.y = random(int(visibleSize.height*0.5), int(visibleSize.height*1.25));
	setPosition(position);
	setAnchorPoint(Vec2(0.8f, 0.5f));
	return true;
}

void Meteorite::update(float duration)
{
	if (m_CurrentState != State::WORK) {
		return;
	}
	GObject::update(duration);
}
