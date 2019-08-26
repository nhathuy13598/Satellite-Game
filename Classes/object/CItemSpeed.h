#ifndef __CITEMSPEED_H__
#define __CITEMSPEED_H__
#include "GObject.h"

class CItemSpeed : public GObject {
public:
	enum AnimType {
		NORMAL_ANIM
	};
	float getRadius();
	static CItemSpeed * createNew();
	void reInit(int _Anim = 0) override;
	void work() override;
	void pause() override;
	void stop() override;
	void update(float duration) override;
	float m_timeLife = 5;

private:
	bool init() override;
	CREATE_FUNC(CItemSpeed);
};

#endif