#ifndef __MAINMENUSCENE_SCENE_H__
#define __MAINMENUSCENE_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "HighScoreScene.h"
#include "GameScene.h"
#include "SceneTutorial.h"
#include "manager\ResourceManager.hpp"
using namespace CocosDenshion;

class MainMenuScene : public cocos2d::Scene
{
public:
	cocos2d::Sprite *background;

    static cocos2d::Scene* createScene();

    virtual bool init();
	void update(float delta) override;
    // a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	void menuHigtScoreCallback(cocos2d::Ref* pSender);
	void menuPlayCallback(cocos2d::Ref* pSender);
	void menuContinueCallback(cocos2d::Ref* pSender);
	void soundEvent(cocos2d::Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenuScene);

public:
	cocos2d::Sprite* m_top;
	cocos2d::Sprite* m_bottom;
	cocos2d::Sprite* m_PlayBox;
	cocos2d::Sprite* m_TutorialBox;
	cocos2d::Sprite* m_HighBox;
	cocos2d::Sprite* m_ExitBox;

	cocos2d::Label* m_PlayText;
	cocos2d::Label* m_TutorialText;
	cocos2d::Label* m_HighText;
	cocos2d::Label* m_ExitText;

	cocos2d::MenuItemImage* soundButton;

	bool soundOn = true;
};

#endif // __MAINMENUSCENE_SCENE_H__
