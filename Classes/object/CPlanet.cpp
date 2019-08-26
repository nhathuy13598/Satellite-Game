#include "CPlanet.h"
#include "cocos2d.h"
#include "tool\Tool.hpp"
#include "GameScene.h"
USING_NS_CC;

#define P_SPEED {400,0}

void CPlanet::setDistance(float distance)
{
	m_Distance = distance;
}

void CPlanet::setNumber(int _number)
{
	m_number = _number;
}

void CPlanet::setMove(bool _move)
{
	m_move = _move;
	if (m_move && m_status == P_GREEN)
	{
		m_Velocity = P_SPEED;
	}
	else m_Velocity = { 0,0 };
}

void CPlanet::setOpsion()
{
	setMove(true);
	collision -= 1;
	if (m_status == 0)
	{
		collisionEffect(getParent());
	}
}

CPlanet * CPlanet::createNew()
{
	return CPlanet::create();
}

//must be called after every settings
void CPlanet::reInit(int _Anim)
{
	stopAllActions();
	removeAllChildren();
	Vec2 m_Origin = Director::getInstance()->getVisibleOrigin();
	Size m_VisibleSize = Director::getInstance()->getVisibleSize();
	//run normal action
	setSpriteFrame(m_Animations[_Anim].front());
	runAction(RepeatForever::create(Animate::create(getAnimation(_Anim))));
	setBlendFunc({ GL_SRC_ALPHA, GL_ONE });

	setAnchorPoint(Vec2(0.5f, 0.5f));
	setAgSpeed(0);
	m_Score = 1;
	collision = 5;
	m_status = _Anim;
}

void CPlanet::work()
{
	m_CurrentState = State::WORK;
	m_collision = true;
	setVisible(true);
}

void CPlanet::pause()
{
	m_CurrentState = State::PAUSE;
}

void CPlanet::stop()
{
	m_CurrentState = State::STOP;
	m_collision = false;
	setVisible(false);
}

Sprite* CPlanet::collisionEffect(Node *_scene)
{
	Sprite* X = Sprite::create();
	if (m_status == 0 && collision == 3) {
		X->initWithFile("img/x1.png");
	}
	if (m_status == 0 && collision == 2) {
		X->initWithFile("img/x2.png");
	}

	if (m_status == 0 && collision == 1) {
		X->initWithFile("img/x3.png");
	}
	
	if (m_status == 0 && collision == 0) {
		stop();
		X->initWithFile("img/x4.png");
	}

	if (collision < 3)
	{
		_scene->removeChild(_scene->getChildByTag(getNumber() * 10 + collision + 1));
	}
	X->setPosition(getPosition());
	X->setAnchorPoint({ 0.5f,0.5f });
	X->setCameraMask((unsigned short)CameraFlag::USER1);
	X->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });
	X->setTag(getNumber()*10+collision);
	_scene->addChild(X);
	return X;
}

bool CPlanet::rePos(Vec2 posTmp)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	auto winSize = Director::getInstance()->getWinSize();

	Vec2 p = getPosition();
	Vec2 cameraPos = m_Camera->getPosition();
	if (p.y < cameraPos.y - getContentSize().width - winSize.width )
	{
		int k = random(0, 4);
		reInit(k);

		setMove(true);

		float height = posTmp.y + visibleSize.width / 1.5;
		float width = random(origin.x + visibleSize.width / 6, origin.x + visibleSize.width - getContentSize().width);
		Vec2 cameraPositoon = { width - (float)cameraPos.x, (float)posTmp.y + (float)visibleSize.width / 1.5f };
		Vec2 worldPosition = m_Camera->cameraToWorld(cameraPositoon);
		setPosition(worldPosition);
		
		work();
		return true;
	}
	return false;
}

bool CPlanet::init()
{
	if (!GObject::init()) {
		return false;
	}
	return true;
}

float CPlanet::getRadius() const
{
	return _contentSize.width / 2;
}

int CPlanet::getNumber() const
{
	return m_number;
}

void CPlanet::update(float duration)
{
	if (m_CurrentState != State::WORK) {
		return;
	}
	Vec2 pos = getPosition();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	if (m_move)
	{
		if ((pos.x  > visibleSize.width))
		{
			m_Velocity = { -400,0 };
		}
		if ((pos.x  < 0))
		{
			m_Velocity = { 400,0 };
		}
	}

	GObject::update(duration);
}

int CPlanet::getCombo(int _combo)
{
	if (m_Score == 0)
	{
		return 0;
	}
	m_Score = 0;
	return _combo+1;
}

int CPlanet::getScore() const
{	
	return m_Score;
}
