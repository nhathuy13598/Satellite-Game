#include "CItemSpeed.h"
#include "tool\Tool.hpp"

USING_NS_CC;

CItemSpeed * CItemSpeed::createNew()
{
	return CItemSpeed::create();
}

//must be called after every settings
void CItemSpeed::reInit(int _Anim)
{
	//run normal action
}

void CItemSpeed::work()
{
	m_CurrentState = State::WORK;
	setVisible(true);
}

void CItemSpeed::pause()
{
	m_CurrentState = State::PAUSE;
}

void CItemSpeed::stop()
{
	m_CurrentState = State::STOP;
	setVisible(false);
}

float CItemSpeed::getRadius()
{
	return _contentSize.width / 2;
}

bool CItemSpeed::init()
{
	if (!GObject::init()) {
		return false;
	}
	setVelocity({ 0,-400 });
	return true;
}

void CItemSpeed::update(float duration)
{
	if (m_CurrentState != State::WORK) {
		return;
	}
	GObject::update(duration);
}

