#ifndef __GANIMATION_3D_CACHE__
#define __GANIMATION_3D_CACHE__
#include <unordered_map>
#include <string>
#include "cocos2d.h"

class GAnimation3DCache
{
public:
    /**get and destroy instance*/
    static GAnimation3DCache* getInstance();
    
    /**get animation by key*/
    cocos2d::Animation3D* getAnimation(const std::string& key);
    
    /**add animation to cache*/
    void addAnimation(const std::string& key, cocos2d::Animation3D* animation);
    
    /**remove all animation*/
    void removeAllAnimations();
    /**remove unused animation*/
    void removeUnusedAnimation();

protected:
    ~GAnimation3DCache();
        
    std::unordered_map<std::string, cocos2d::Animation3D*> _animations; //cached animations
};
#endif