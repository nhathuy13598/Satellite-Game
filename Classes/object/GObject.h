#ifndef __GOBJECT_H__
#define __GOBJECT_H__
#include "cocos2d.h"
#include "object/GCamera.h"
#include "manager\ResourceManager.hpp"
#include <vector>
#include <unordered_map>
#include <string>

class GObject : public cocos2d::Sprite {
public:
	enum class State {
		WORK, STOP, PAUSE
	};
	virtual void setVelocity(const cocos2d::Vec2 & vec);
	virtual void setAcceleration(const cocos2d::Vec2 & ac);
	virtual void setCamera(GCamera *_camera);
	virtual GCamera* getCamera();
	virtual void stopMoveAndRotate();
	virtual void stopMove();
	virtual void setAnim(int type, const std::vector<std::string>& anims, float fps = 1);
	virtual void setAnim2(int type, char *format, int numFrame, int start, int count, float fps);
	virtual void reInit(int _Anim = 0) = 0;	//must be called after every settings
	virtual void work() = 0;
	virtual void pause() = 0;
	virtual void stop() = 0;
	virtual State getState();
	virtual ~GObject() {}

	void update(float duration) override;
	void setAgSpeed(float ag);
	cocos2d::Vec2 getVelocity() const;
	cocos2d::Vec2 getAcceleration() const;
	float getAgSpeed() const;
	std::unordered_map<int, cocos2d::Vector<cocos2d::SpriteFrame*>> m_Animations;
	std::unordered_map<int, float> m_AnimFPS;
	cocos2d::Animation* getAnimation(int type, float fps = 0);

protected:

	
	
protected:

	static const float ROT_MIN;
	static const float MOV_MIN;
	static const float ACC_MIN;

	
	float m_AgSpeed = 0;
	float m_CurrentTime = 0;
	cocos2d::Vec2 m_Velocity{0, 0};
	cocos2d::Vec2 m_Acceleration{ 0, 0 };
	cocos2d::Vec2 m_Pivot{0, 1};
	State m_CurrentState = State::WORK;
	GCamera *m_Camera;
};
#endif