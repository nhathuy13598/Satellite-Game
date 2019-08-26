#ifndef __CPLANET_H__
#define __CPLANET_H__
#include "GObject.h"
#include "cocos2d.h"
#include <vector>

class CPlanet : public GObject {
public:
	enum AnimType {
		NORMAL_ANIM1, NORMAL_ANIM2, NORMAL_ANIM3, NORMAL_ANIM4, NORMAL_ANIM5
	};

	void setMove(bool _move);
	void setOpsion();
	static CPlanet* createNew();
	
	int getCombo(int _combo);
	int getScore() const;
	float getRadius() const;
	int getNumber() const;
	bool rePos(cocos2d::Vec2 posTmp);
	void setDistance(float distance);
	void setNumber(int _number);
	Sprite* collisionEffect(cocos2d::Node *_scene);
	
	void reInit(int _Anim = 0) override;
	void work() override;
	void pause() override;
	void stop() override;
	void update(float duration) override;
public:
	
	int m_status;
	bool m_move = false;
	bool m_collision = true;
	int collision = 5;
	//float m_timeLife = 3;
	
private:
	bool init() override;
	CREATE_FUNC(CPlanet);
private:
	int m_number;
	int m_Score = 1;
	float m_Distance = 0;
	float m_CharAgSpeed = 270;
};

#endif