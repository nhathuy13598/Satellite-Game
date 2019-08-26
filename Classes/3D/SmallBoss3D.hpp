#ifndef __SMALL_BOSS_H__
#define __SMALL_BOSS_H__
#include "GObject3D.hpp"
#include <string>

class SmallBoss3D : public GObject3D {
public:
	enum AnimType {
		NORMAL_ANIM, DEAD_ANIM
	};
	SmallBoss3D();
	float getRadius();
	static SmallBoss3D * create(const char * model);
	static SmallBoss3D* create(const char* model, const char * texture);
	void reInit() override;
	void setPoint(int point);
	void work() override;
	void pause() override;
	void stop() override;
	void setRotationZ(float degree);
	void setRotationZQuat(const cocos2d::Quaternion &quat);
	int getPoint();
	std::string link;

private:
	void update(float duration) override;
private:
	CREATE_FUNC(SmallBoss3D);
	float m_CurrentTime = 0;
	cocos2d::Quaternion m_RotZQuaternion;
	int m_Point = 3;
};

#endif