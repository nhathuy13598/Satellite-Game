#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__
#include "cocos2d.h"
#include "object\GCamera.h"
#include "manager\ResourceManager.hpp"
USING_NS_CC;

class BackGround : public Node
{
public:
	static BackGround * createNew();
	void reInit();
	void update(float duration) override;
public:
	Sprite *wallLeft1, *wallRight1, *wallLeft2, *wallRight2;
	Sprite *m_Background, *midBackground, *botBackgournd;
	ParticleSystemQuad *LineRight, *LineLeft;
	Vec2 z;
	GCamera* m_Camera;
	Size m_Size;

private:
	bool init() override;
	CREATE_FUNC(BackGround);
};

#endif