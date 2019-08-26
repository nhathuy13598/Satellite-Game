#ifndef __IUSE_COLLIDING_TRACKER__
#define __IUSE_COLLIDING_TRACKER__

class IUseCollidingTracker {
public:
	virtual void onHaveColliding(GObject * obj1, GObject * obj2) = 0;
};

#endif