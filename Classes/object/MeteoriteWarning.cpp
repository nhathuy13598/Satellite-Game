#include "MeteoriteWarning.hpp"
#include "tool\Tool.hpp"
#include "GameScene.h"

USING_NS_CC;

MeteoriteWarning * MeteoriteWarning::createNew()
{
	return MeteoriteWarning::create();
}

//must be called after every settings
void MeteoriteWarning::reInit(int _Anim)
{
	if (m_Animations.size() == 0) {
		return;
	}
	//run normal action
	auto particleX = ParticleSystemQuad::create(RESOURCE(R_R_particle_warning_plist));
	particleX->setVisible(true);
	particleX->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
	particleX->setTag(1);
	addChild(particleX);

	setSpriteFrame(m_Animations[NORMAL_ANIM].front());
	runAction(RepeatForever::create(Animate::create(getAnimation(NORMAL_ANIM))));
}

void MeteoriteWarning::setDisplayInCameraBorder(GCamera * camera, 
	const cocos2d::Vec2 & metePosition, 
	const cocos2d::Vec2 & target, float paddingX, float paddingY)
{
	m_Camera = camera;
	m_MeteoritePosition = metePosition;
	m_Target = target;
	m_PaddingX = getContentSize().width / 2 + paddingX;
	m_PaddingY = getContentSize().height / 2 + paddingY;
}

void MeteoriteWarning::work()
{
	m_CurrentState = State::WORK;
	setVisible(true);
}

void MeteoriteWarning::pause()
{
	m_CurrentState = State::PAUSE;
}

void MeteoriteWarning::stop()
{
	m_CurrentState = State::STOP;
	setVisible(false);
}

bool MeteoriteWarning::init()
{
	if (!GObject::init()) {
		return false;
	}
	setVisible(false);
	return true;
}

void MeteoriteWarning::update(float duration)
{
	if (m_CurrentState != State::WORK) {
		return;
	}
	GObject::update(duration);
	////located in camera bound
	//Size visibleSize = Director::getInstance()->getVisibleSize();
	//Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//Rect rect{ m_Camera->getPosition(), visibleSize };
	//*setPosition(mtl::findPointOfRectLine(rect, m_Target, m_MeteoritePosition, m_PaddingX, m_PaddingY));*/
}
