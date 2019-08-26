#ifndef __GAMESCENE_SCENE_H__
#define __GAMESCENE_SCENE_H__

#include "cocos2d.h"
#include "object\CPlanet.h"
#include "object\CItemSpeed.h"
#include "object\CCharacter.h"
#include "object\GCamera.h"
#include "object\Meteorite.h"
#include "object\BackGround.h"
#include "object\CArrow.h"
#include "SimpleAudioEngine.h"
#include "3D\SmallBoss3D.hpp"
#include "manager\UserData.hpp"
#include "object\MeteoriteWarning.hpp"
#include "fragment\IFragment.hpp"
#include "object\Particle.h"
#include "fragment\IFragmentBase.hpp"
#include <deque>

class GameSceneUI;
USING_NS_CC;
static bool isTutorial = true;

struct FragmentSlot {
	int m_NumPlanets = 0;
	bool m_HaveMeteorites = false;
	bool m_IsMapAttached = false;
	IFragment * m_Fragment = nullptr;
	FragmentSlot * next = nullptr;
};

class GameScene : public cocos2d::Scene, public IFragmentBase
{
public:
	enum class State {
		START, WORK, STOP
	};
	enum Fragment {
		FRAGMENT_1, FRAGMENT_2, FRAGMENT_3
	};
	//test
	IFragment * fragment;
	IFragment * fragment2;
	bool flag = true;
	bool flag1 = true;
	//--test
	static cocos2d::Scene* createScene();
	// a selector callback
	void menuSceneCallback(Ref * pSender);
	void menuReplayCallback(Ref * pSender);
	/* IFragmentBase methods */
	float getScore() override;
	void setScore(float score) override;
	void addScore(float amount) override;
	GCamera * getCamera() override;
	GObject * getCharacter() override;
	void addUI(cocos2d::Node * ui) override;
	void setGameOver() override;
	void start();
	void start(cocos2d::Vec2 pos);
	void work();
	void stop();
	void onEnter() override;
	void addFragment(IFragment * fragment, int numPlanets, bool haveMeteorites);
private:
	//methods
	virtual bool init() override;
	void initWorld();
	void initUI();
	void initEventListeners();
	void update(float) override;
	bool isGameOver();
	bool isMainCharOutOfCamera();
	/* Tat de thu UI
	void upScore(int _score);
	*/
	bool findCollisionPlanet();
	bool isCollideWithMeteorites();
	void generateMeteorites();
	void rotateCharAroundPlanet(float duration);
	void generatePlanets();
	void calculateMeteoritesVelocity(Meteorite * met);
	/* Tat de thu UI
	void updateScoreUI();
	*/
	void updateAliveLimit();
	void runEasyStage(float duration);
	void runMeteoriteStage(float duration);
	void generateMeteoriteWarning();
	void goSceneTutorial();
	void setArrow(CArrow * arrow, const cocos2d::Vec2 &pos, const cocos2d::Vec2 &dir);
	void stopAlertArrow();
	void updateLayout(bool haveMeteorites);
	//Show what's in visible screen
	void filterVisibleObjects();
	CREATE_FUNC(GameScene);
	//callback
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event *event);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event* event);
	// TypewritingEffect
	void type(float dt);
	//update all
	void updateAll(float duration);
	void onFiringCharacter();
	cocos2d::Rect getVisibleRect();
	void runFragment();
private:
	//save value from config
	double m_MainToPeekTime;
	double m_MainToPeekWinMulitply;
	double m_MainSelfRotationSpeed;
	double m_MainRotationSpeed;

	//save size, origin from director
	Size m_VisibleSize;
	Size m_WinSize;
	Vec2 m_Origin;

	UserData* m_UserData;
	BackGround *m_Background;
	MeteoriteWarning* m_MeteWarning = nullptr;

	int m_Combo = -1;
	int m_Score = 0;
	bool m_GameOver = false;
	bool m_StartGame = false;
	bool m_IsActive = true;
	bool m_HaveUpdateLayout = true;
	//obstacles
	bool m_HaveBigObstacle = false;
	//All GObject
	std::vector<GObject*> m_GObjects;
	std::deque<CItemSpeed*> m_items;
	//Camera
	GCamera* m_Camera;
	//Character
	CCharacter* m_Character = nullptr;
	//planets
	std::deque<CPlanet*> m_Planets;
	CPlanet* m_temp;
	//meteorites
	std::deque<Meteorite*> m_Meteorites;
	//current planet
	CPlanet* m_CurrentPlanet = nullptr;
	//Camera limit inside a Rect
	cocos2d::Rect m_CameraLimit;
	//alive rect
	cocos2d::Rect m_AliveLimit;
	Node * world;
	//Button
	Menu *menuReplay /*, *menuBack*/;

	//Main properties
	float m_CharacterAgSpeed = 180;
	float m_CharacterRotationTime = 0;
	//Meteorite properties
	float m_NumRoundsForComing = 1;
	float m_DistanceWhenComing;

	// Tutorial
	cocos2d::Sprite* m_DialBox;
	cocos2d::Label* m_Message1;
	int i = 0;

	//arrrow
	CArrow * m_Arrow;
	CArrow * m_AlertArrow;
	//State
	State m_CurrentState;

	//UI
	GameSceneUI* m_UI;
	//start at
	cocos2d::Vec2 m_StartAt;

	//Planets counting
	int m_PlanetCount = 0;
};
#endif // __GAMESCENE_SCENE_H__
