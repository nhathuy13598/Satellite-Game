#ifndef __POINT_FRAGMENT__
#define __POINT_FRAGMENT__
#include <unordered_map>
#include <string>
#include "object\SmallPoint.h"
#include "IFragment.hpp"
/*
This map contains Planets, SmallPoints, DirectionalPoints, optionally may have somes obstacles
*/

class IFragmentBase;
class CCharacter;
class SmallBoss3D;
class CPlanet;
class GCamera;

class PointFragment : public IFragment {
public:
	
	static PointFragment * createNew(const char * map);
	void initMap(const char * map);
	//IFragment methods
	void start() override;
	void resume() override;
	void pause() override;
	void stop() override;
	void reset() override;
	void update(float duration) override;
	void setBase(IFragmentBase * base) override;
	cocos2d::Size getSize() override;
	IFragment::State getState() override;
	cocos2d::Rect getRect() override;
private:
	bool init() override;
	void reInit();
	void initEventListeners();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event *event);
	bool findCollisionPlanet();
	bool findColiisionPoint();
	void updateAliveLimit();
	void filterVisibleObjects();
	void rotateCharAroundPlanet(float duration);
	bool isMainCharOutOfCamera();
	void updateAll(float duration);
	void onEnter() override;
	CREATE_FUNC(PointFragment)
private:
	IFragmentBase * m_Base;
	std::unordered_map<std::string, CPlanet*> m_Planets;
	std::unordered_map<std::string, SmallPoint*> m_DirectionalPoints;
	std::unordered_map<std::string, SmallPoint*> m_SmallPoints;
	CCharacter * m_Character;
	GCamera * m_Camera;
	CPlanet * m_CurrentPlanet;
	float m_CharacterAgSpeed = 180;
	float m_MainToPeekWinMulitply;
	float m_MainToPeekTime;
	//save size, origin from director
	cocos2d::Size m_VisibleSize;
	cocos2d::Size m_WinSize;
	cocos2d::Vec2 m_Origin;
	//Camera limit inside a Rect
	cocos2d::Rect m_CameraLimit;
	//alive rect
	cocos2d::Rect m_AliveLimit;
	//State
	State m_CurrentState = IFragment::State::START;
	//
	cocos2d::Size m_MapSize;
};
#endif