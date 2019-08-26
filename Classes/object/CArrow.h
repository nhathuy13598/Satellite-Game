#ifndef __CARROW_H__
#define __CARROW_H__
#include "GObject.h"
class CArrow : public GObject {
public:
	enum AnimType {
		NORMAL_ANIM
	};
	float getRadius();
	static CArrow * createNew();
	void reInit(int _Anim = 0) override;
	void work() override;
	void pause() override;
	void stop() override;
	void update(float duration) override;

private:
	bool init() override;
	CREATE_FUNC(CArrow);
};

#endif