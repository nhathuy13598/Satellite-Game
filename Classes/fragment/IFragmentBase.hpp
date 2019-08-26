#ifndef __IFRAGMENT_BASE_HPP__
#define __IFRAGMENT_BASE_HPP__
#include "../object/GObject.h"
#include "../object/GCamera.h"
#include "object\Particle.h"
#include "cocos2d.h"
class IFragmentBase {
public:
	virtual float getScore() = 0;
	virtual void setScore(float score) = 0;
	virtual void addScore(float amount) = 0;
	virtual GCamera * getCamera() = 0;
	virtual GObject * getCharacter() = 0;
	virtual void addUI(cocos2d::Node * ui) = 0;
	virtual void setGameOver() = 0;
};
#endif