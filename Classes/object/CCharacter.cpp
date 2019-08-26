#include "CCharacter.h"
#include "tool\Tool.hpp"

USING_NS_CC;

CCharacter * CCharacter::createNew()
{
	return CCharacter::create();
}



//must be called after every settings
void CCharacter::reInit(int _Anim)
{
	//run normal action
	setSpriteFrame(m_Animations[_Anim].front());
	runAction(RepeatForever::create(Animate::create(getAnimation(_Anim))));

	this->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->setAgSpeed(400);

	//Yellow: 0
	addParticle(RESOURCE(R_R_particle_yellowPlanet_plist));

	//Green: 1-4
	addParticle(RESOURCE(R_R_particle_greenPlanet_plist));
	addParticle(RESOURCE(R_R_particle_greenPlanet1_plist));
	addParticle(RESOURCE(R_R_particle_greenPlanet2_plist));
	addParticle(RESOURCE(R_R_particle_greenPlanet3_plist));

	//Blue: 5
	addParticle(RESOURCE(R_R_particle_bluePlanet_plist));

	//Red: 6-10
	addParticle(RESOURCE(R_R_particle_redPlanet1_plist));
	addParticle(RESOURCE(R_R_particle_redPlanet2_plist));
	addParticle(RESOURCE(R_R_particle_redPlanet3_plist));
	addParticle(RESOURCE(R_R_particle_redPlanet4_plist));
	addParticle(RESOURCE(R_R_particle_redPlanet5_plist));
	//Lighting: 11
	addParticle(RESOURCE(R_R_particle_lightingPlanet_plist));
	setStatus(_Anim);
}

void CCharacter::addParticle(const char *format)
{
	auto particleX = ParticleSystemQuad::create(format);
	particleX->setVisible(false);
	particleX->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	particleX->setTag(m_numStatus);
	this->addChild(particleX, -1);
	m_numStatus++;
}

void CCharacter::work()
{
	m_CurrentState = State::WORK;
	setVisible(true);
}

void CCharacter::pause()
{
	m_CurrentState = State::PAUSE;
}

void CCharacter::stop()
{
	m_CurrentState = State::STOP;
	setVisible(false);
}
float CCharacter::getRadius() const
{
	return _contentSize.width / 2;
}

bool CCharacter::init()
{
	if (!GObject::init()) {
		return false;
	}
	return true;
}

void CCharacter::update(float duration)
{
	if (m_CurrentState != State::WORK) {
		return;
	}
	m_timeStatus += duration;
	if (m_status == C_RED && m_timeStatus < 5)
	{
		setStatus(C_RED + int(m_timeStatus));
	}
	if (m_status == C_GREEN && m_timeStatus < 4)
	{
		setStatus(C_GREEN + int(m_timeStatus));
	}
	GObject::update(duration);
}

void CCharacter::covertStatus(int k)
{
	if (k == 0) m_status = C_YELLOW;
	if (k == 1) m_status = C_GREEN;
	if (k == 2) m_status = C_BLUE;
	if (k == 3) m_status = C_RED;
	if (k == 4) m_status = C_LIGHTING;
	setStatus(m_status);
}

void CCharacter::setStatus(int k)
{
	//m_status = k;
	for (int i = 0; i < m_numStatus; i++)
	{
		getChildByTag(i)->setVisible(false);
	}
	getChildByTag(k)->setVisible(true);

}
