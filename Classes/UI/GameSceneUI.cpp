#include "GameSceneUI.h"

static bool isTutorialUI = true;
cocos2d::Label *m_scoreLabel = nullptr;
cocos2d::Label *m_HighScoreLabel = nullptr;
cocos2d::Label *m_ComboLabel = nullptr;
GameSceneUI* GameSceneUI::createNew()
{
	return GameSceneUI::create();
}

// on "init" you need to initialize your instance
bool GameSceneUI::init()
{
	if (!Node::init())
	{
		return false;
	}
	//Visible size, origin, win size
	m_VisibleSize = _director->getVisibleSize();
	m_Origin = _director->getVisibleOrigin();
	m_WinSize = _director->getWinSize();

	//save data from config
	scheduleUpdate();
	return true;
}


void GameSceneUI::reInit(int Score, int highestScore)
{

	//Back to menu button
	auto labelBack = Label::createWithTTF("back", RESOURCE(R_R_f_ethno_ttf), 50);
	labelBack->setColor(Color3B::WHITE);
	auto buttonBack = new MenuItemLabel();
	buttonBack->initWithLabel(labelBack, CC_CALLBACK_1(GameSceneUI::menuSceneCallback, this));
	menuBack = Menu::create(buttonBack, NULL);
	menuBack->setPosition(m_Origin.x + m_VisibleSize.width - buttonBack->getContentSize().width / 2, 
		m_Origin.y + buttonBack->getContentSize().height / 2);
	menuBack->setVisible(true);
	this->addChild(menuBack, 1);

	//Score Label
	__String *tempScore = __String::createWithFormat("%i", Score);
	m_scoreLabel = Label::createWithTTF(tempScore->getCString(), RESOURCE(R_R_f_ethno_ttf), m_VisibleSize.width / 14);
	m_scoreLabel->setColor(Color3B(255, 255, 255));
	m_scoreLabel->setPosition(m_Origin.x + m_VisibleSize.width - m_scoreLabel->getContentSize().width / 2 - m_VisibleSize.width / 80,
		m_Origin.y + m_VisibleSize.height - m_scoreLabel->getContentSize().height / 2);
	addChild(m_scoreLabel, 1);

	//Combo score label
	__String *_combo = __String::createWithFormat("%i +", Score);
	m_ComboLabel = Label::createWithTTF(_combo->getCString(), RESOURCE(R_R_f_comic_zine_ot_otf), m_VisibleSize.width / 8);
	m_ComboLabel->setColor(Color3B(255, 255, 255));
	m_ComboLabel->setPosition(m_Origin.x + m_VisibleSize.width / 2 - m_ComboLabel->getContentSize().width / 2 - m_VisibleSize.width / 80,
		m_Origin.y + m_VisibleSize.height / 2 - m_ComboLabel->getContentSize().height / 2);
	addChild(m_ComboLabel, 1);
	m_ComboLabel->setVisible(false);

	//--- label Score
	__String *highScore = __String::createWithFormat("%i", highestScore);
	m_HighScoreLabel = Label::createWithTTF(highScore->getCString(), RESOURCE(R_R_f_ethno_ttf), m_VisibleSize.width / 14);
	m_HighScoreLabel->setColor(Color3B(255, 255, 255));
	m_HighScoreLabel->setPosition(m_Origin.x + m_HighScoreLabel->getContentSize().width / 2,
		m_Origin.y + m_VisibleSize.height - m_HighScoreLabel->getContentSize().height / 2);
	addChild(m_HighScoreLabel, 1);

	//Play/Pause button
	playButton = MenuItemImage::create();
	playButton->setNormalImage(Sprite::create("img/Pause.png"));
	playButton->setCallback(CC_CALLBACK_1(GameSceneUI::PauseCallback, this));
	PlayPause = Menu::create(playButton,nullptr);
	playButton->setAnchorPoint(Vec2(0,0));
	PlayPause->setPosition(m_Origin.x, m_Origin.y);
	addChild(PlayPause);
}

void GameSceneUI::updateScoreUI(int score, int highestScore, int combo)
{
	__String *tempScore = __String::createWithFormat("%i", score);
	m_scoreLabel->setString(tempScore->getCString());
	m_scoreLabel->setPosition(m_Origin.x + m_VisibleSize.width - m_scoreLabel->getContentSize().width / 2 - m_VisibleSize.width / 80,
		m_Origin.y + m_VisibleSize.height - m_scoreLabel->getContentSize().height / 2);
	m_scoreLabel->stopAllActions();
	m_scoreLabel->runAction(Sequence::createWithTwoActions(ScaleTo::create(0.1f, 1.5f), ScaleTo::create(0.1f, 1.0f)));

	__String *highScore = __String::createWithFormat("%i", highestScore);
	m_HighScoreLabel->setString(highScore->getCString());
	m_HighScoreLabel->setPosition(m_Origin.x + m_HighScoreLabel->getContentSize().width / 2,
		m_Origin.y + m_VisibleSize.height - m_HighScoreLabel->getContentSize().height / 2);

	__String *_combo = __String::createWithFormat("%i", combo);
	m_ComboLabel->setString(_combo->getCString());
	m_ComboLabel->setPosition(m_Origin.x + m_VisibleSize.width / 2, m_Origin.y + m_VisibleSize.height / 1.5);
	m_ComboLabel->runAction(Sequence::createWithTwoActions(ScaleTo::create(0.1f, 1.f + MIN(8.f,combo / 5.f)), ScaleTo::create(0.1f, 1.0f)));
	m_ComboLabel->runAction(Sequence::createWithTwoActions(FadeIn::create(0.8f), FadeOut::create(0.8f)));
}
void GameSceneUI::upScore(int &score, int combo, UserData* user)
{

	if (combo > 0)
	{
		score += combo;
		m_ComboLabel->setVisible(true);
	}
	else
	{
		m_ComboLabel->setVisible(false);
	}
	if (score > user->getHighScore()) {
		user->setHighScore(score);
		user->save();
	}
	this->updateScoreUI(score, user->getHighScore(), combo);
}
void GameSceneUI::menuSceneCallback(Ref* pSender)
{
	if (isPause == false) {
		_director->pushScene(MainMenuScene::create());
	}
}


void GameSceneUI::PauseCallback(Ref * pSender)
{
	if (PlayPause->isEnabled() && isPause == false) {
		playButton->setNormalImage(Sprite::create("img/Play.png"));
		this->getParent()->pause();
		isPause = true;
	}
	else if (isPause == true) {
		isPause = false;
		playButton->setNormalImage(Sprite::create("img/Pause.png"));
		this->getParent()->resume();
	}
}


