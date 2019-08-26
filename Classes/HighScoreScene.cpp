#include "HighScoreScene.h"
#include "SimpleAudioEngine.h"
#include "MainMenuScene.h"
#include "manager/UserData.hpp"

USING_NS_CC;

Scene* HighScoreScene::createScene()
{
    return HighScoreScene::create();
}

static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in LoadingScene.cpp\n");
}

static float y;

bool HighScoreScene::init()
{

    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto label = Label::createWithTTF("high score", RESOURCE(R_R_f_spac_ttf), 80);
	label->setTextColor(Color4B::YELLOW);
    label->setPosition(Vec2(origin.x + visibleSize.width/2, 
							origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);

	__String *tempScore = __String::createWithFormat("%i", UserData::getInstance()->getHighScore());
	auto m_scoreLabel = Label::createWithTTF(tempScore->getCString(), RESOURCE(R_R_f_Spac3Italic_ttf), visibleSize.width / 8);
	m_scoreLabel->setColor(Color3B::WHITE);
	m_scoreLabel->setPosition(origin.x + visibleSize.width/2 , origin.y + visibleSize.height/1.5 - m_scoreLabel->getContentSize().height / 1.7);
	addChild(m_scoreLabel, 1);


    background = Sprite::create(RESOURCE(R_R_IMG_bgHS_jpg));
	background->setAnchorPoint({ 0, 0 });
	Texture2D::TexParams texParams;
	texParams.wrapS = GL_REPEAT;
	texParams.wrapT = GL_REPEAT;
	background->getTexture()->setTexParameters(texParams);
	background->setTextureRect({ 0, 0, visibleSize.width, visibleSize.height });
	addChild(background);

	//Button Back
	auto labelBack = Label::createWithTTF("back", RESOURCE(R_R_f_spac_ttf), 50);
	labelBack->setColor(Color3B::BLACK);
	auto buttonBack = new MenuItemLabel();
	buttonBack->initWithLabel(labelBack, CC_CALLBACK_1(HighScoreScene::menuSceneCallback, this));
	auto menuBack = Menu::create(buttonBack, NULL);
	menuBack->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 6));
	this->addChild(menuBack, 1);
	this->scheduleUpdate();
    return true;
}

void HighScoreScene::update(float delta)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	background->setTextureRect({ y -= 0.25, 0 , visibleSize.width, visibleSize.height });
}

void HighScoreScene::menuSceneCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, MainMenuScene::createScene()));
}
