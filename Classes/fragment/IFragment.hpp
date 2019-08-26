#ifndef __FRAGMENT__
#define __FRAGMENT__
#include "cocos2d.h"

class IFragmentBase;

class IFragment : public cocos2d::Node {
public:
	enum class State {
		START, RESUME, PAUSE, STOP
	};

	virtual void start() = 0;
	virtual void resume() = 0;
	virtual void pause() = 0;
	virtual void stop() = 0;
	virtual void reset() = 0;
	virtual void setBase(IFragmentBase * base) = 0;
	virtual cocos2d::Size getSize() = 0;
	virtual State getState() = 0;
	virtual cocos2d::Rect getRect() = 0;
};

#endif