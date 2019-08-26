#ifndef __GAMESCENEUI_H__
#define __GAMESCENEUI_H__
#include "GameScene.h"
#include "MainMenuScene.h"
class GameSceneUI : public cocos2d::Node
{
public:
	static GameSceneUI * createNew();
	void reInit(int Score, int highScore);
	void menuSceneCallback(Ref * pSender);
	void PauseCallback(Ref *pSender);
	void updateScoreUI(int score, int highestScore, int combo);
	void upScore(int &score, int combo, UserData* user);
	cocos2d::Label* getScoreLabel() { return m_scoreLabel; }
	cocos2d::Label* getHighScoreLabel() { return m_HighScoreLabel; }
	cocos2d::Label* getComboLabel() { return m_ComboLabel; }


	Menu *menuBack, *PlayPause;

	
	Size m_VisibleSize;
	Size m_WinSize;
	
	cocos2d::Label *m_scoreLabel;
	cocos2d::Label *m_HighScoreLabel;
	cocos2d::Label *m_ComboLabel;
	
	Vec2 m_Origin;
	
	bool isPause = false;
	MenuItemImage* playButton;
	
private:
	bool init() override;
	CREATE_FUNC(GameSceneUI);
};
#endif // __GAMESCENEUI_H__