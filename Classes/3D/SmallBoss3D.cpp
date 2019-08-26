#include "SmallBoss3D.hpp"
#include "..\tool\Tool.hpp"

USING_NS_CC;

SmallBoss3D::SmallBoss3D() : GObject3D() {
}

SmallBoss3D * SmallBoss3D::create(const char * model)
{
	auto boss = new (std::nothrow) SmallBoss3D;
	if (boss && boss->initWithFile(model))
	{
		boss->_contentSize = boss->getBoundingBox().size;
		boss->autorelease();
		return boss;
	}
	CC_SAFE_DELETE(boss);
	return nullptr;
}

SmallBoss3D* SmallBoss3D::create(const char* model, const char* texture)
{
	auto boss = create(model);
	if (boss)
	{
		boss->setTexture(model);
	}
	return boss;
}

void SmallBoss3D::reInit()
{
	if (!m_Animations.size()) {
		return;
	}
	Animation3D* normalAnim = m_Animations[NORMAL_ANIM];
	if (normalAnim) {
		stopAllActions();
		runAction(RepeatForever::create(Animate3D::create(normalAnim)));
	}
}

void SmallBoss3D::setPoint(int point)
{
	m_Point = point;
}

int SmallBoss3D::getPoint()
{
	return m_Point;
}

float SmallBoss3D::getRadius()
{
	return _contentSize.width / 2;
}

void SmallBoss3D::work()
{
	m_CurrentState = State::WORK;
	setVisible(true);
}

void SmallBoss3D::pause()
{
	m_CurrentState = State::PAUSE;
}

void SmallBoss3D::stop()
{
	m_CurrentState = State::STOP;
	setVisible(false);
}

void SmallBoss3D::setRotationZ(float degree)
{
	m_RotZQuaternion = Quaternion({0, 0, 1}, CC_DEGREES_TO_RADIANS(degree));
}

void SmallBoss3D::setRotationZQuat(const cocos2d::Quaternion & quat)
{
	m_RotZQuaternion = quat;
}


void SmallBoss3D::update(float duration)
{
	if (m_CurrentState != State::WORK) {
		return;
	}
	if (m_AgSpeed < -ROT_MIN || m_AgSpeed > ROT_MIN) {	
		Quaternion quat{ m_RotAxis, CC_DEGREES_TO_RADIANS(m_AgSpeed) * m_CurrentTime };
		setRotationQuat(m_RotZQuaternion * quat);
	}

	if (mtl::isVecOutRange(-MOV_MIN, MOV_MIN, m_Velocity) ||
		mtl::isVecOutRange(-ACC_MIN, ACC_MIN, m_Acceleration)) {
		Vec3 dv = duration * m_Acceleration;
		setPosition3D(getPosition3D() + duration * m_Velocity + duration * dv / 2);
		m_Velocity += dv;
	}
	m_CurrentTime += duration;
}

