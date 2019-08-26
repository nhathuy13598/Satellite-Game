#ifndef __METEORITE_H__
#define __METEORITE_H__
#include "GObject.h"

class Meteorite : public GObject {
public:
	enum AnimType {
		NORMAL_ANIM
	};

	static Meteorite* createNew();

	float getRadius();
	bool rePos();
	void reInit(int _Anim = 0) override;
	void work() override;
	void pause() override;
	void stop() override;
	void update(float duration) override;

private:
	bool init() override;
	CREATE_FUNC(Meteorite);
};

#endif