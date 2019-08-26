#include "GObjectCollidingTracker.hpp"

USING_NS_CC;

bool GObjectCollidingTracker::isCharWithPlanets(CCharacter * character, std::deque<CPlanet*>& planets)
{
	for (CPlanet *planet : planets) {
		Vec2 dir = character->getPosition() - planet->getPosition();
		if (planet->getBoundingBox().containsPoint(character->getPosition()) && planet->m_collision) {
			(m_HandlerOwner->*m_Handler)(character, planet);
			return true;
		}
	}
	return false;
}

bool GObjectCollidingTracker::isOneWithN(GObject * obj1, std::deque<GObject*>& obj2s)
{
	bool haveCollision = false;
	for (GObject *obj2 : obj2s) {
		if (obj1->getBoundingBox().containsPoint(obj2->getPosition())) {
			(m_HandlerOwner->*m_Handler)(obj1, obj2);
			haveCollision = true;
		}
	}
	return haveCollision;
}

bool GObjectCollidingTracker::isNWithN(std::deque<GObject*>& obj1s, std::deque<GObject*>& obj2s)
{
	bool haveCollision = false;
	for (GObject *obj1 : obj1s) {
		for (GObject *obj2 : obj2s) {
			if (obj1->getBoundingBox().containsPoint(obj2->getPosition())) {
				(m_HandlerOwner->*m_Handler)(obj1, obj2);
				haveCollision = true;

			}
		}
	}
	return haveCollision;
}

void GObjectCollidingTracker::setHandler(void(IUseCollidingTracker::*handler)(GObject *, GObject *), IUseCollidingTracker * owner)
{
	m_HandlerOwner = owner;
	this->m_Handler = handler;
}
