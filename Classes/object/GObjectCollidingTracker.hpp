#ifndef __GOBJECT_COLLIDING_TRACKER_HPP__
#define __GOBJECT_COLLIDING_TRACKER_HPP__

#include <vector>
#include <deque>
#include <utility>
#include "CCharacter.h"
#include "CPlanet.h"
#include "cocos2d.h"
#include "IUseCollidingTracker.hpp"

class GObjectCollidingTracker {
public:
	bool isCharWithPlanets(CCharacter * main, std::deque<CPlanet*> &planets);
	bool isOneWithN(GObject * obj1, std::deque<GObject*> & obj2s);
	bool isNWithN(std::deque<GObject*> & obj1s, std::deque<GObject*> & obj2s);
	void setHandler(void (IUseCollidingTracker::*handler)(GObject*, GObject*), IUseCollidingTracker * owner);
public:
	void (IUseCollidingTracker::*m_Handler)(GObject*, GObject*);
	IUseCollidingTracker * m_HandlerOwner;
	std::vector<std::pair<GObject*, GObject*>> m_Pairs;

};

#endif