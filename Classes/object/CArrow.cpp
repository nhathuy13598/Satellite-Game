#include "CArrow.h"
#include "tool\Tool.hpp"

USING_NS_CC;

CArrow * CArrow::createNew()
{
	return CArrow::create();
}

//must be called after every settings
void CArrow::reInit(int _Anim)
{
	//run normal action
	setSpriteFrame(m_Animations[_Anim].front());
	runAction(RepeatForever::create(Animate::create(getAnimation(_Anim))));

	scheduleUpdate();
}

void CArrow::work()
{
	m_CurrentState = State::WORK;
	setVisible(true);
}

void CArrow::pause()
{
	m_CurrentState = State::PAUSE;
}

void CArrow::stop()
{
	m_CurrentState = State::STOP;
	setVisible(false);
}

float CArrow::getRadius()
{
	return _contentSize.width / 2;
}

bool CArrow::init()
{
	if (!GObject::init()) {
		return false;
	}
	return true;
}

void CArrow::update(float duration)
{
	if (m_CurrentState != State::WORK) {
		return;
	}
	GObject::update(duration);
}
