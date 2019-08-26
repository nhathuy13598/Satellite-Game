#ifndef __GOBJECT3D_H__
#define __GOBJECT3D_H__
#include "cocos2d.h"
#include "../object/GCamera.h"
#include <initializer_list>
#include <vector>

class GObject3D : public cocos2d::Sprite3D {
public:
	enum class State {
		WORK, PAUSE, STOP
	};
	virtual void setVelocity(const cocos2d::Vec3 & vec);
	virtual void setAcceleration(const cocos2d::Vec3 & ac);
	virtual void setAgSpeed(float ag);
	virtual void setRotationAxis(const cocos2d::Vec3 & axis);
	virtual void setAnim(int key, const char * animName, float speed = 1);
	virtual void stopMoveAndRotation();
	virtual void stopMove();
	virtual State getState();
	virtual ~GObject3D() {}
	virtual void reInit() = 0;	//must be called after every settings
	virtual void work() = 0;
	virtual void pause() = 0;
	virtual void stop() = 0;

	GObject3D();
	cocos2d::Vec3 getVelocity() const;
	cocos2d::Vec3 getAcceleration() const;
protected:
	static const float ROT_MIN;
	static const float MOV_MIN;
	static const float ACC_MIN;

	std::unordered_map<int, cocos2d::Animation3D*> m_Animations;
	std::unordered_map<int, float> m_AnimSpeed;
	float m_AgSpeed = 0;
	State m_CurrentState = State::WORK;
	cocos2d::Vec3 m_RotAxis{ 0, 1, 0 };
	cocos2d::Vec3 m_Velocity{0, 0, 0};
	cocos2d::Vec3 m_Acceleration{ 0, 0 , 0};
	cocos2d::Vec3 m_Pivot{0, 1, 0};
};
#endif