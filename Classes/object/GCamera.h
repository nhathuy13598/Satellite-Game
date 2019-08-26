#ifndef __GCAMERA_H__
#define __GCAMERA_H__
#include "cocos2d.h"

class GCamera : public cocos2d::Camera
{
public:
	static GCamera * createNew();
	static GCamera * createPerspective(float degree, float ratio, float near, float far);
	static GCamera * createOrthographic(float width, float height, float near, float far);
	void setVelocity(const cocos2d::Vec2 &vec);
	void setAcceleration(const cocos2d::Vec2 &ac);
	void stopMove();
	cocos2d::Vec2 worldToCamera(const cocos2d::Vec2 &pos);
	cocos2d::Vec2 cameraToWorld(const cocos2d::Vec2 &pos);
	cocos2d::Vec2 getVeclocity();
	void update(float duration) override;

private:
	static const float MOV_MIN;
	static const float ACC_MIN;
	bool init() override;
	CREATE_FUNC(GCamera);

	cocos2d::Vec2 m_Velocity{ 0,0 };
	cocos2d::Vec2 m_Acceleration{ 0,0 };
};

#endif