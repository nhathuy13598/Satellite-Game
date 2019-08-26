#ifndef __CCHARACTER_H__
#define __CCHARACTER_H__
#include "GObject.h"
#include "object\CPlanet.h"

class CCharacter : public GObject {
public:
	enum AnimType {
		NORMAL_ANIM
	};
	float getRadius() const;
	static CCharacter * createNew();

	void addParticle(const char *format);
	void reInit(int _Anim = 0) override;
	void work() override;
	void pause() override;
	void stop() override;
	void update(float duration) override;
	void covertStatus(int k);
	void setStatus(int k);
	float m_timeLife = 5;
	int m_numStatus = 0;
	float m_timeStatus = 0;
	int m_status = 0;

private:
	bool init() override;
	CREATE_FUNC(CCharacter);
};

#endif