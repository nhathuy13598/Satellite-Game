#include "AppDelegate.h"
#include "MainMenuScene.h"
#include "manager\ResourceManager.hpp"
#include "manager\UserData.hpp"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize;
//static cocos2d::Size smallResolutionSize = cocos2d::Size(540, 960);
//static cocos2d::Size mediumResolutionSize = cocos2d::Size(750, 1334);
//static cocos2d::Size largeResolutionSize = cocos2d::Size(1080, 1920);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 2};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
	//init manager
	ResourceManager* resManager = ResourceManager::getInstance();
	resManager->init();
	
	//init userDa
	UserData* userData = UserData::getInstance();
	userData->init();

	//design resolution
	designResolutionSize = 
		cocos2d::Size(CONFIG(R_designResWidth), CONFIG(R_designResHeight));
    // initialize director
    auto director = Director::getInstance(); 
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("MyGame",
			cocos2d::Rect(0, 0, CONFIG(R_desktopWidth), CONFIG(R_desktopHeight)));
#else
        glview = GLViewImpl::create("Mygame");
#endif
        director->setOpenGLView(glview);
    }
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);
	director->setDisplayStats(false);
    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
    auto frameSize = glview->getFrameSize();

    register_all_packages();

	//load plist
	//Planet
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(RESOURCE(R_R_If1_plist));

	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(RESOURCE(R_R_Pf1_plist));
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(RESOURCE(R_R_Pf3_plist));
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(RESOURCE(R_R_Pf4_plist));
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(RESOURCE(R_R_Pf5_plist));
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(RESOURCE(R_R_Pf6_plist));
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(RESOURCE(R_R_Pf7_plist));
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(RESOURCE(R_R_Pf8_plist));
	//Charactor
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(RESOURCE(R_R_Cf1_plist));
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(RESOURCE(R_R_Cf2_plist));
	//Meteorite
	cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(RESOURCE(R_R_Mf1_plist));
    // create a scene. it's an autorelease object
    //auto scene = MainMenuScene::createScene();
	auto scene = MainMenuScene::createScene();
	// run
    director->runWithScene(scene);
    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
	AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
