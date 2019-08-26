#include "Particle.h"


Particle * Particle::createNew()
{
	return Particle::create();
}

void Particle::reInit()
{

}

Particle * Particle::particleOneTime(Vec2 pos, const char *format, char *nameTag, float time , int _camera, int z)
{
	
	auto particleX = ParticleSystemQuad::create(format);
	particleX->setPosition(pos);
	particleX->setVisible(true);
	if (_camera == 1)
	{
		particleX->setCameraMask((unsigned short)CameraFlag::USER1);
	}
	else particleX->setCameraMask((unsigned short)CameraFlag::DEFAULT);
	if (nameTag)
	{
		particleX->setName(nameTag);
	}
	addChild(particleX, z);
	runAction(Sequence::create(DelayTime::create(time), RemoveSelf::create(), NULL));
	return this;
}

Particle *Particle::particlePlanet(CPlanet *planet)
{
	const char *format;
	if (planet->m_status == 0)
		format = RESOURCE(R_R_particle_collision1_plist);
	if (planet->m_status == 1)
		format = RESOURCE(R_R_particle_collision3_plist);
	if (planet->m_status == 2)
		format = RESOURCE(R_R_particle_collision2_plist);
	if (planet->m_status == 3)
		format = RESOURCE(R_R_particle_collision4_plist);
	if (planet->m_status == 4)
		format = RESOURCE(R_R_particle_collision5_plist);
	particleOneTime(planet->getPosition(), format);
	return this;
}

Particle *Particle::particleCharDie(CCharacter *character)
{
	const char *format;
	if (character->m_status == C_YELLOW)
		format = RESOURCE(R_R_particle_yellowDie_plist);
	if (character->m_status == C_GREEN)
		format = RESOURCE(R_R_particle_greenDie_plist);
	if (character->m_status == C_BLUE)
		format = RESOURCE(R_R_particle_blueDie_plist);
	if (character->m_status >= C_RED)
		format = RESOURCE(R_R_particle_redDie_plist);
	particleOneTime(character->getPosition(), format);
	return this;
}


bool Particle::init()
{
	return true;	
}

void Particle::update(float duration)
{

}
