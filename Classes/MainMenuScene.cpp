#include "MainMenuScene.h"

#define COLOR_TEXT_LABEL cocos2d::Color3B(0, 193, 255)
#define GAME_TEXT_LABEL cocos2d::Color3B(152, 213, 184)
static float x;

Scene* MainMenuScene::createScene()
{
    return MainMenuScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in LoadingScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto label = Label::createWithTTF(STRING(R_gameNameLabel), RESOURCE(R_R_f_simson_ttf), 150);
	label->setColor(GAME_TEXT_LABEL);
    if (label == nullptr)
    {
        problemLoading(RESOURCE(R_R_f_Menu_ttf));
    }
    else
    {
        label->setPosition(Vec2(visibleSize.width/2,
                                visibleSize.height - label->getContentSize().height));
		label->runAction(DelayTime::create(0.5));
		label->runAction(RepeatForever::create(Sequence::create(TintTo::create(1,Color3B::WHITE),
			TintTo::create(1, COLOR_TEXT_LABEL),
			TintTo::create(1,Color3B::WHITE),nullptr)));
        this->addChild(label, 1);
    }

	background = Sprite::create(RESOURCE(R_R_IMG_bgMenu_jpg));
	background->setAnchorPoint({ 0, 0 });
	Texture2D::TexParams texParams;
	texParams.wrapS = GL_REPEAT;
	texParams.wrapT = GL_REPEAT;
	background->getTexture()->setTexParameters(texParams);
	background->setTextureRect({ 0, 0, visibleSize.width, visibleSize.height});
	auto particleC = ParticleSystemQuad::create(RESOURCE(R_R_particle_light1_plist));
	particleC->setVisible(true);
	particleC->setAnchorPoint(Vec2(0, 0));
	particleC->setPosition(origin);
	background->addChild(particleC, 10);
	addChild(background);
	//----------------------------------------------
	
	// Tao menu
	m_PlayBox = Sprite::create("img/title.png");
	m_PlayBox->setBlendFunc({ GL_SRC_ALPHA,GL_ONE });
	auto boxSize = m_PlayBox->getContentSize();
	m_top = Sprite::create("img/top.png");
	m_top->setBlendFunc({ GL_SRC_ALPHA,GL_ONE });
	auto topSize = m_top->getContentSize();
	m_top->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + topSize.height / 2));
	m_top->runAction(Sequence::create(DelayTime::create(0.1f), MoveTo::create(0.2f, Vec2(visibleSize.width / 2, 
		visibleSize.height / 2 + topSize.height * 3)), nullptr));
	m_bottom = Sprite::create("img/bottom.png");
	m_bottom->setBlendFunc({ GL_SRC_ALPHA,GL_ONE });
	m_bottom->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - topSize.height / 2));
	m_bottom->runAction(Sequence::create(DelayTime::create(0.1f), MoveTo::create(0.2f, Vec2(visibleSize.width / 2, 
		visibleSize.height / 2 - topSize.height * 5 - boxSize.height / 10)),nullptr));
	

	// Play
	m_PlayText = Label::createWithTTF("play", RESOURCE(R_R_f_ethno_ttf), 80);
	m_PlayText->setColor(COLOR_TEXT_LABEL);
	m_PlayText->runAction(DelayTime::create(0.5));
	m_PlayText->runAction(RepeatForever::create(Sequence::create(FadeTo::create(2,125),FadeTo::create(2,255),nullptr)));
	auto PlayButton = MenuItemLabel::create(m_PlayText, CC_CALLBACK_1(MainMenuScene::menuContinueCallback, this));
	auto MenuPlay = Menu::create(PlayButton, NULL);
	MenuPlay->setPosition(Vec2(boxSize.width / 2, boxSize.height / 2));
	MenuPlay->setOpacity(0);
	MenuPlay->runAction(Sequence::create(DelayTime::create(0.3f), FadeIn::create(0.2f), nullptr));
	m_PlayBox->addChild(MenuPlay);
	m_top->addChild(m_PlayBox);
	m_PlayBox->setPosition(Vec2(-boxSize.width / 2, -boxSize.height / 2 - boxSize.height / 10));
	m_PlayBox->setOpacity(0);
	m_PlayBox->runAction(Sequence::create(DelayTime::create(0.3f),
		Spawn::create(FadeIn::create(0.2f), MoveTo::create(0.2f,Vec2(topSize.width / 2, -boxSize.height / 2 - boxSize.height / 10)),nullptr),
		nullptr));

	// Tutorial
	m_TutorialBox = Sprite::create("img/title.png");
	m_TutorialBox->setBlendFunc({ GL_SRC_ALPHA,GL_ONE });
	m_TutorialText = Label::createWithTTF("help", RESOURCE(R_R_f_ethno_ttf), 80);
	m_TutorialText->setColor(COLOR_TEXT_LABEL);
	m_TutorialText->runAction(DelayTime::create(0.5));
	m_TutorialText->runAction(RepeatForever::create(Sequence::create(FadeTo::create(2, 125), FadeTo::create(2, 255), nullptr)));
	auto TutorialButton = MenuItemLabel::create(m_TutorialText, CC_CALLBACK_1(MainMenuScene::menuPlayCallback, this));
	auto MenuTutorial = Menu::create(TutorialButton, NULL);
	MenuTutorial->setPosition(Vec2(boxSize.width / 2, boxSize.height / 2));
	MenuTutorial->setOpacity(0);
	MenuTutorial->runAction(Sequence::create(DelayTime::create(0.3f), FadeIn::create(0.2f), nullptr));
	m_TutorialBox->addChild(MenuTutorial);
	m_top->addChild(m_TutorialBox);
	m_TutorialBox->setPosition(Vec2(topSize.width + boxSize.width / 2, -boxSize.height - boxSize.height / 2 - boxSize.height / 5));
	m_TutorialBox->setOpacity(0);
	m_TutorialBox->runAction(Sequence::create(DelayTime::create(0.3f),
		Spawn::create(FadeIn::create(0.2f), MoveTo::create(0.2f, Vec2(topSize.width / 2, -boxSize.height - boxSize.height / 2 - boxSize.height / 5)), nullptr),
		nullptr));

	// High score
	m_HighBox = Sprite::create("img/title.png");
	m_HighBox->setBlendFunc({ GL_SRC_ALPHA,GL_ONE });
	m_HighText = Label::createWithTTF("score", RESOURCE(R_R_f_ethno_ttf), 80);
	m_HighText->setColor(COLOR_TEXT_LABEL);
	m_HighText->runAction(DelayTime::create(0.5));
	m_HighText->runAction(RepeatForever::create(Sequence::create(FadeTo::create(2, 125), FadeTo::create(2, 255), nullptr)));
	auto HighButton = MenuItemLabel::create(m_HighText, CC_CALLBACK_1(MainMenuScene::menuHigtScoreCallback, this));
	auto MenuHigh = Menu::create(HighButton, NULL);
	MenuHigh->setPosition(Vec2(boxSize.width / 2, boxSize.height / 2));
	MenuHigh->setOpacity(0);
	MenuHigh->runAction(Sequence::create(DelayTime::create(0.3f), FadeIn::create(0.2f), nullptr));
	m_HighBox->addChild(MenuHigh);
	m_top->addChild(m_HighBox);
	m_HighBox->setPosition(Vec2(-boxSize.width / 2, -boxSize.height * 2 - boxSize.height / 2 - boxSize.height * 3 / 10));
	m_HighBox->setOpacity(0);
	m_HighBox->runAction(Sequence::create(DelayTime::create(0.3f),
		Spawn::create(FadeIn::create(0.2f), MoveTo::create(0.2f, Vec2(topSize.width / 2, -boxSize.height * 2 - boxSize.height / 2 - boxSize.height * 3 / 10)), nullptr),
		nullptr));

	// Exit
	m_ExitBox = Sprite::create("img/title.png");
	m_ExitBox->setBlendFunc({ GL_SRC_ALPHA,GL_ONE });
	m_ExitText = Label::createWithTTF("exit", RESOURCE(R_R_f_ethno_ttf), 80);
	m_ExitText->setColor(COLOR_TEXT_LABEL);
	m_ExitText->runAction(DelayTime::create(0.5));
	m_ExitText->runAction(RepeatForever::create(Sequence::create(FadeTo::create(2, 125), FadeTo::create(2, 255), nullptr)));
	auto ExitButton = MenuItemLabel::create(m_ExitText, CC_CALLBACK_1(MainMenuScene::menuCloseCallback, this));
	auto MenuExit = Menu::create(ExitButton, NULL);
	MenuExit->setPosition(Vec2(boxSize.width / 2, boxSize.height / 2));
	MenuExit->setOpacity(0);
	MenuExit->runAction(Sequence::create(DelayTime::create(0.3f), FadeIn::create(0.2f), nullptr));
	m_ExitBox->addChild(MenuExit);
	m_top->addChild(m_ExitBox);
	m_ExitBox->setPosition(Vec2(topSize.width + boxSize.width / 2, -boxSize.height * 3 - boxSize.height / 2 - boxSize.height * 2 / 5));
	m_ExitBox->setOpacity(0);
	m_ExitBox->runAction(Sequence::create(DelayTime::create(0.3f),
		Spawn::create(FadeIn::create(0.2f), MoveTo::create(0.2f, Vec2(topSize.width / 2, -boxSize.height * 3 - boxSize.height / 2 - boxSize.height * 2 / 5)), nullptr),
		nullptr));

	this->addChild(m_top, 10);
	this->addChild(m_bottom, 10);


	
	// Them background music
	auto theme = SimpleAudioEngine::getInstance();
	theme->preloadBackgroundMusic(RESOURCE(R_R_S_Menu_mp3));
	theme->preloadEffect(RESOURCE(R_R_S_Bleep_01_mp3));
	theme->playBackgroundMusic(RESOURCE(R_R_S_Menu_mp3), true);
	auto volume = UserData::getInstance()->getAudio();
	theme->setBackgroundMusicVolume(volume);
	theme->setEffectsVolume(volume);

	// Them setting sound
	soundButton = MenuItemImage::create();
	if (1 == volume) {
		soundButton->setNormalImage(Sprite::create("img/soundon.png"));
		soundOn = true;
	}
	else if (0 == volume) {
		soundButton->setNormalImage(Sprite::create("img/soundoff.png"));
		soundOn = false;
	}
	
	soundButton->setCallback(CC_CALLBACK_1(MainMenuScene::soundEvent, this));
	auto menuSound = Menu::create(soundButton, nullptr);
	soundButton->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	menuSound->setPosition(Vec2(origin.x, origin.y));
	this->addChild(menuSound);

	this->scheduleUpdate();
    return true;
}


void MainMenuScene::update(float delta)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	background->setTextureRect({ x -= 0.25, 0, visibleSize.width, visibleSize.height});
	background->setPosition(origin);
}

void MainMenuScene::menuHigtScoreCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	SimpleAudioEngine::getInstance()->playEffect(RESOURCE(R_R_S_Bleep_01_mp3));
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, HighScoreScene::create()));
}

void MainMenuScene::menuPlayCallback(cocos2d::Ref * pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	SimpleAudioEngine::getInstance()->playEffect(RESOURCE(R_R_S_Bleep_01_mp3));
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, TutorialScene::createScene()));
}

void MainMenuScene::menuContinueCallback(cocos2d::Ref * pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	SimpleAudioEngine::getInstance()->playEffect(RESOURCE(R_R_S_Bleep_01_mp3));
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, GameScene::createScene()));
}

void MainMenuScene::soundEvent(cocos2d::Ref * pSender)
{
	if (soundOn == true) {
		soundOn = false;
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
		SimpleAudioEngine::getInstance()->setEffectsVolume(0);
		UserData::getInstance()->setAudio(0);
		UserData::getInstance()->save();
		soundButton->setNormalImage(Sprite::create("img/soundoff.png"));
	}
	else {
		soundOn = true;
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
		SimpleAudioEngine::getInstance()->setEffectsVolume(1);
		UserData::getInstance()->setAudio(1);
		UserData::getInstance()->save();
		soundButton->setNormalImage(Sprite::create("img/soundon.png"));
	}
}

void MainMenuScene::menuCloseCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	SimpleAudioEngine::getInstance()->playEffect(RESOURCE(R_R_S_Bleep_01_mp3));
    Director::getInstance()->end();
}
