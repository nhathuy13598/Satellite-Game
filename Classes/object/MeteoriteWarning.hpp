#ifndef __METEORITE_WARNING__
#define __METEORITE_WARNING__
#include "GObject.h"
#include "GCamera.h"

class MeteoriteWarning : public GObject {
public:
	enum AnimType {
		NORMAL_ANIM
	};

	static MeteoriteWarning * createNew();

	void reInit(int _Anim = 0) override;
	void setDisplayInCameraBorder(GCamera * camera,
		const cocos2d::Vec2 &metePosition, 
		const cocos2d::Vec2 &target, float paddingX = 0, float paddingY = 0);
	void work() override;
	void pause() override;
	void stop() override;
	void update(float duration) override;

private:
	bool init() override;
	CREATE_FUNC(MeteoriteWarning);
private:

	cocos2d::Vec2 m_Target;
	cocos2d::Vec2 m_MeteoritePosition;
	float m_PaddingX = 0;
	float m_PaddingY = 0;
	float m_CurrentTime = 0;
};

#endif 