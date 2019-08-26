#include "GAnimation3DCache.hpp"

USING_NS_CC;

GAnimation3DCache* GAnimation3DCache::getInstance()
{
	static GAnimation3DCache cache;
    return &cache;
}

Animation3D* GAnimation3DCache::getAnimation(const std::string& key)
{
    auto it = _animations.find(key);
    if (it != _animations.end())
        return it->second;
    return nullptr;
}

void GAnimation3DCache::addAnimation(const std::string& key, Animation3D* animation)
{
    const auto& it = _animations.find(key);
    if (it != _animations.end())
    {
        return; // already have this key
    }
    _animations[key] = animation;
    animation->retain();
}

void GAnimation3DCache::removeAllAnimations()
{
    for (auto itor : _animations) {
        CC_SAFE_RELEASE(itor.second);
    }
    _animations.clear();
}
void GAnimation3DCache::removeUnusedAnimation()
{
    for (auto itor = _animations.begin(); itor != _animations.end(); ) {
        if (itor->second->getReferenceCount() == 1)
        {
            itor->second->release();
            itor = _animations.erase(itor);
        }
        else
            ++itor;
    }
}

GAnimation3DCache::~GAnimation3DCache()
{
    removeAllAnimations();
}
