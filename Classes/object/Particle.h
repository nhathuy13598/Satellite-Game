#ifndef __PARTICLE_H__
#define __PARTICLE_H__
#include "cocos2d.h"
#include "object\GCamera.h"
#include "manager\ResourceManager.hpp"
#include "object\CPlanet.h"
#include "object\CCharacter.h"

USING_NS_CC;

class Particle : public Node
{
public:
	static Particle * createNew();
	void reInit();


	Particle * particlePlanet(CPlanet * planet);
	Particle * particleCharDie(CCharacter * planet);
	Particle * particleOneTime(Vec2 pos, const char *format, char *nameTag = nullptr, float time = 5.0f, int _camera = 1 , int z = 10);

private:

	bool init() override;
	void update(float duration) override;
	CREATE_FUNC(Particle);
};

#endif