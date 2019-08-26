#ifndef __SMALLPOINT_H__ 
#define __SMALLPOINT_H__
#include "GObject.h"
#include "cocos2d.h"
#include <vector>

class SmallPoint : public GObject {
public:
	enum AnimType {
		NORMAL_ANIM1, NORMAL_ANIM2, NORMAL_ANIM3, NORMAL_ANIM4, NORMAL_ANIM5
	};
	static SmallPoint* createNew();

	Sprite* collisionEffect(cocos2d::Scene *_scene);

	void reInit(int _Anim = 0) override;
	void work() override;
	void pause() override;
	void stop() override;
	void addParticle(const char * format);
	void update(float duration) override;
	void setPoint(int point);
	int getPoint();
	std::string link;

private:
	int m_Point = 3;
	bool init() override;
	CREATE_FUNC(SmallPoint);


};

#endif