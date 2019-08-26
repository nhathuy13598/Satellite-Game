#ifndef __TUTORIAL_SCENE_H__
#define __TUTORIAL_SCENE_H__

#include "cocos2d.h"
#include "object\CPlanet.h"
#include "object\CCharacter.h"
#include "object\GCamera.h"
#include "object\Meteorite.h"
#include "object\BackGround.h"
#include "object\CArrow.h"
#include "SimpleAudioEngine.h"
#include "3D\SmallBoss3D.hpp"
#include "manager\UserData.hpp"
#include "object\MeteoriteWarning.hpp"
#include <deque>

USING_NS_CC;
class TutorialScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();
	void menuSceneCallback(Ref * pSender);
	void actionFinish();
private:
	virtual bool init() override;
	Node* initWorldObjects();
	void updateAll(float duration);
	void initSceneObjects();
	void initEventListener();
	void update(float) override;
	void gameOver();
	bool isMainCharOutOfCamera();
	bool findCollisionPlanet();
	bool isCollideWithMeteorites();
	void generateMeteorites();
	void rotateCharAroundPlanet(float duration);
	void calculateMeteoritesVelocity(Meteorite * met);
	void updateAliveLimit();
	void runEasyStage(float duration);
	void runMeteoriteStage(float duration);
	void generateMeteoriteWarning();
	CREATE_FUNC(TutorialScene);
	//callback
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event *event);

	// TypewritingEffect
	void type(float dt);

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

	bool m_GameOver = false;
	bool m_StartGame = false;
	//meteorites
	std::deque<Meteorite*> m_Meteorites;

	//Camera
	GCamera* m_Camera;
	//Character
	CCharacter* m_Character = nullptr;
	//planets
	Vector<CPlanet*> m_Planets;
	//current planet
	CPlanet* m_CurrentPlanet = nullptr;
	//Camera limit inside a Rect
	cocos2d::Rect m_CameraLimit;
	//alive rect
	cocos2d::Rect m_AliveLimit;
	//Distance
	float m_CurrentState = 0;
	Node * world;
	//Button
	Menu *menuBack;

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
	bool isTutorial = true;
	bool swap = false;
	Vec2 veloTemp;
	//-- End Tutorial

	// Flag
	int count = 0; // Dem so vong quay cua character
	bool firstJump = true;
	bool secondJump = false;
	bool thirdJump = false;
	bool finishTutorial = false;

	// Tutorial Sprite
	Sprite* m_Click;

	// End label
	Label* m_label;
};
#endif // __TUTORIAL_SCENE_H__
