#ifndef __HIGHSCORESCENE_SCENE_H__
#define __HIGHSCORESCENE_SCENE_H__

#include "cocos2d.h"

class HighScoreScene : public cocos2d::Scene
{
public:
	cocos2d::Sprite *background;

    static cocos2d::Scene* createScene();

    virtual bool init();

	void update(float delta) override;

	//void update(float delta) override;
    
    // a selector callback
    void menuSceneCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HighScoreScene);
};

#endif // __HIGHSCORESCENE_SCENE_H__
