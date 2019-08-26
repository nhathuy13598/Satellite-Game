#include "BackGround.h"

BackGround * BackGround::createNew()
{
	return BackGround::create();
}

void BackGround::reInit()
{

}

bool BackGround::init()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	this->setPosition(Vec2(0, 0));
	z = Vec2(0, 0);
	//background
	m_Background = Sprite::create(RESOURCE(R_R_bg2_jpg));
	m_Background->setAnchorPoint({ 0, 0 });
	Texture2D::TexParams texParams;
	texParams.wrapS = GL_REPEAT;
	texParams.wrapT = GL_REPEAT;
	m_Background->getTexture()->setTexParameters(texParams);
	m_Background->setTextureRect({ { -visibleSize.width / 2, 0 }, visibleSize * 2 });
	this->addChild(m_Background, 0);

	auto particleC = ParticleSystemQuad::create(RESOURCE(R_R_particle_light_plist));
	particleC->setVisible(true);
	particleC->setAnchorPoint(Vec2(0, 0));
	particleC->setPosition(Vec2(0, 0));
	m_Background->addChild(particleC, 10);

	LineRight = ParticleSystemQuad::create(RESOURCE(R_R_particle_lineWall_plist));
	LineRight->setVisible(true);
	LineRight->setAnchorPoint(Vec2(0, 0));
	LineRight->setPosition(Vec2(origin.x + visibleSize.width*1.5, origin.y));

	LineLeft = ParticleSystemQuad::create(RESOURCE(R_R_particle_lineWall_plist));
	LineLeft->setVisible(true);
	LineLeft->setAnchorPoint(Vec2(0, 0));
	LineLeft->setPosition(Vec2(origin.x - visibleSize.width*0.5, origin.y));

	addChild(LineLeft);
	addChild(LineRight);
	//*-* Wall
	return true;
}

static float multiply = 20;

void BackGround::update(float duration)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float rectWidh = visibleSize.width / 20;
	Rect visibleRect{ m_Camera->getPosition(), visibleSize };
	Rect LineLeftRect{ LineLeft->getPosition(),{ rectWidh, visibleSize.height } };
	Rect LineRightRect{ LineRight->getPosition(),{ rectWidh, visibleSize.height } };
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Vec2 cameraVeclocity = m_Camera->getVeclocity();
	Vec2 backgroundVelocity = cameraVeclocity * duration / multiply;
	if (visibleRect.intersectsRect(LineLeftRect)) {
		LineLeft->setVisible(true);
	}
	else {
		LineLeft->setVisible(false);
	}
	if (visibleRect.intersectsRect(LineRightRect)) {
		LineRight->setVisible(true);
	}
	else {
		LineRight->setVisible(false);
	}
	z += {backgroundVelocity.x, -backgroundVelocity.y};
	m_Background->setTextureRect({ z, visibleSize * 2 });
	m_Background->setPosition(m_Camera->getPosition() - Vec2(visibleSize.width*0.1, 0));
	LineLeft->setPositionY(m_Camera->getPosition().y);
	LineRight->setPositionY(m_Camera->getPosition().y);
}