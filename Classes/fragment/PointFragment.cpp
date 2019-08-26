#include "PointFragment.hpp"
#include "../manager/ResourceManager.hpp"
#include "../object/CCharacter.h"
#include "../object/CPlanet.h"
#include "../object/SmallPoint.h"
#include "../3D/SmallBoss3D.hpp"
#include "../tool/Tool.hpp"
#include "../object/GCamera.h"
#include "IFragmentBase.hpp"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

PointFragment * PointFragment::createNew(const char * map)
{
	PointFragment * fragment = PointFragment::create();
	if (fragment) {
		fragment->initMap(map);
	}
	return fragment;
}

void PointFragment::start()
{
	m_CurrentState = IFragment::State::START;
	setVisible(true);
}

//init some event
void PointFragment::resume()
{
	m_CurrentState = IFragment::State::RESUME;
}

void PointFragment::pause()
{
	m_CurrentState = IFragment::State::PAUSE;
}

void PointFragment::stop()
{
	m_CurrentState = IFragment::State::STOP;
	setVisible(false);
}

void PointFragment::reset()
{
	m_Character = nullptr;
}

void PointFragment::initMap(const char * map)
{
	TMXTiledMap * tileMap = new TMXTiledMap();
	tileMap->initWithTMXFile(map);
	Size mapTiles = tileMap->getMapSize();
	Size tileSize = tileMap->getTileSize();
	m_MapSize.width = mapTiles.width * tileSize.width;
	m_MapSize.height = mapTiles.height * tileSize.height;
	// get Planets
	CCTMXObjectGroup* planetLayer = tileMap->objectGroupNamed("Planets");
	auto objects = planetLayer->getObjects();
	int numObjects = objects.size();
	for (int i = 0; i < numObjects; i++) {
		auto map = objects[i].asValueMap();
		auto x = map["x"].asFloat();
		auto y = map["y"].asFloat();
		std::string name = map["name"].asString();
		CPlanet *planet = CPlanet::createNew();
		planet->setAnim(CPlanet::NORMAL_ANIM5, FRAMES(R_A_PLANET_4), 30);
		planet->setAnim(CPlanet::NORMAL_ANIM1, FRAMES(R_A_PLANET_5), 30);
		planet->setAnim(CPlanet::NORMAL_ANIM2, FRAMES(R_A_PLANET_6), 30);
		planet->setAnim(CPlanet::NORMAL_ANIM3, FRAMES(R_A_PLANET_7), 30);
		planet->setAnim(CPlanet::NORMAL_ANIM4, FRAMES(R_A_PLANET_8), 30);
		planet->reInit(4);
		planet->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });
		planet->setPosition(x, y);
		planet->setAgSpeed(0);
		planet->work();
		addChild(planet);
		m_Planets[name] = planet;
	}

	// Get small points
	CCTMXObjectGroup* smallPointLayer = tileMap->objectGroupNamed("SmallPoints");
	objects = smallPointLayer->getObjects();
	numObjects = objects.size();
	for (int i = 0; i < numObjects; i++) {
		auto map = objects[i].asValueMap();
		float x = map["x"].asFloat();
		float y = map["y"].asFloat();
		std::string name = map["name"].asString();

		SmallPoint * point = SmallPoint::createNew();
		point->reInit();
		point->addParticle(RESOURCE(R_R_particle_smallPoint2_plist));
		point->addParticle(RESOURCE(R_R_particle_smallPointEffect_plist));
		point->setPosition({ x, y});
		point->setPoint(3);
		point->work();
		addChild(point);
		m_SmallPoints[name] = point;
	}

	// Get directional points
	CCTMXObjectGroup* directionalPointLayer = tileMap->objectGroupNamed("DirectionalPoints");
	objects = directionalPointLayer->getObjects();
	numObjects = objects.size();
	for (int i = 0; i < numObjects; i++) {
		auto map = objects[i].asValueMap();
		float x = map["x"].asFloat();
		float y = map["y"].asFloat();
		std::string name = map["name"].asString();
		std::string link = map["link"].asString();
		SmallPoint * point = SmallPoint::createNew();
		if (!link.empty()) {
			point->link = link;
		}
		point->reInit();
		point->addParticle(RESOURCE(R_R_particle_smallPoint_plist));
		point->addParticle(RESOURCE(R_R_particle_smallPointEffect_plist));
		point->setPosition({ x, y });
		point->setPoint(3);
		point->work();
		addChild(point);
		m_DirectionalPoints[name] = point;
	}
}

bool PointFragment::init()
{
	if (!IFragment::init()) {
		return false;
	}
	m_VisibleSize = _director->getVisibleSize();
	m_Origin = _director->getVisibleOrigin();
	m_WinSize = _director->getWinSize();
	return true;
}

void PointFragment::reInit()
{
	//character
	m_Character = static_cast<CCharacter*>(m_Base->getCharacter());
	//camera
	m_Camera = m_Base->getCamera();
	//Camrea position is limited inside a specific Rect, its position is relative to camera
	Vec2 centerOfRect{ m_Origin.x + m_VisibleSize.width / 2, m_Origin.y + m_VisibleSize.height / 3 };
	Size sizeOfRect{ m_VisibleSize.width / 5, m_VisibleSize.width / 5 };
	m_CameraLimit = { centerOfRect.x - sizeOfRect.width / 2, centerOfRect.y - sizeOfRect.height, sizeOfRect.width, sizeOfRect.height };
	//Alive limit, its x limit is relative to world, its y limit is relative to camera
	m_AliveLimit = { m_Origin.x - m_VisibleSize.width, m_Origin.y, m_VisibleSize.width * 3, m_VisibleSize.height };
	//save data from config
	ResourceManager* resManager = ResourceManager::getInstance();
	m_MainToPeekTime = resManager->getConfig(R_mainToPeekTime);
	m_MainToPeekWinMulitply = resManager->getConfig(R_mainToPeekWinMultiply);
}

void PointFragment::initEventListeners()
{
	EventListenerTouchOneByOne *touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(PointFragment::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool PointFragment::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	if (m_CurrentState != IFragment::State::RESUME || !m_CurrentPlanet) {
		return false;
	}
	//SimpleAudioEngine::getInstance()->playEffect(RESOURCE(R_R_S_Jump_mp3));
	Vec2 dir = convertToNodeSpace(m_Character->getPosition()) - m_CurrentPlanet->getPosition();
	dir.normalize();
	float s = m_WinSize.width * m_MainToPeekWinMulitply;
	float accelerationLength = -2 * s / (m_MainToPeekTime * m_MainToPeekTime);
	float velocityLength = -accelerationLength * m_MainToPeekTime;
	Vec2 velocity = dir * velocityLength;
	Vec2 acceleration = dir * accelerationLength;
	m_Character->setVelocity(velocity);
	m_Character->setAcceleration(acceleration);
	m_Camera->setVelocity(velocity);
	m_Camera->setAcceleration(acceleration);
	m_CurrentPlanet = nullptr;
	return true;
}

bool PointFragment::findCollisionPlanet()
{
	if (m_CurrentPlanet) {
		return false;
	}
	//find collision planet and save it
	//increase score if passing planets
	for (auto pair : m_Planets) {
		CPlanet * planet = pair.second;
		Vec2 characterPos = convertToNodeSpace(m_Character->getPosition());
		Vec2 dir = characterPos - planet->getPosition();
		if (planet->isVisible() && planet->getBoundingBox().containsPoint(characterPos)) {
			float dirLength = dir.length();
			float charRadius = m_Character->getRadius();
			float distance = planet->getRadius() + charRadius + charRadius / 4;
			if (dirLength <= distance) {
				Particle *parC = Particle::createNew();
				addChild(parC->particleOneTime(planet->getPosition(), RESOURCE(R_R_particle_collision_plist)));
				//find rotation direction
				Vec2 mainVelocity = m_Character->getVelocity();
				Vec3 mainDir3D{ mainVelocity.x, mainVelocity.y, 0 };
				Vec3 toPlanetDir3D{ dir.x, dir.y, 0 };
				Vec3 normal;
				Vec3::cross(toPlanetDir3D, mainDir3D, &normal);
				if (normal.z >= 0) {
					m_CharacterAgSpeed = 270;
				}
				else {
					m_CharacterAgSpeed = -270;
				}
				dir = dir * distance / dirLength;
				m_Character->stopMove();
				m_Camera->stopMove();
				m_Character->setPosition(convertToWorldSpace(dir + planet->getPosition()));
				m_CurrentPlanet = planet;
				return true;
			}
		}
	}
	return false;
}

bool PointFragment::findColiisionPoint()
{
	if (m_CurrentPlanet) {
		return false;
	}
	for (auto pair : m_DirectionalPoints) {
		SmallPoint * point = pair.second;
		if (point->getState() == GObject::State::STOP) {
			continue;
		}
		Vec2 characterPos = convertToNodeSpace(m_Character->getPosition());
		Vec2 dir = characterPos - point->getPosition();
			float dirLength = dir.length();
			float charRadius = m_Character->getRadius();
			float distance = m_Character->getRadius() * 1.5f;
			if (dirLength <= distance) {
				point->stop();
				addChild(Particle::createNew()->particleOneTime(point->getPosition(), RESOURCE(R_R_particle_star_plist)));
				m_Base->addScore(point->getPoint());
				m_Character->setPosition(convertToWorldSpace(point->getPosition()));
				if (!point->link.empty()) {
					SmallPoint * linkPoint = m_DirectionalPoints[point->link];
					if (linkPoint->getState() == GObject::State::STOP) {
						continue;
					}
					Vec2 dir = linkPoint->getPosition() - point->getPosition();
					dir.normalize();
					m_Character->stopMove();
					m_Character->setVelocity(dir * m_VisibleSize.width * 1.8f);
					m_Camera->stopMove();
					m_Camera->setVelocity(dir * m_VisibleSize.width * 1.8f);
				}	
				return true;
			}
	}
	for (auto pair : m_SmallPoints) {
		SmallPoint * point = pair.second;
		if (point->getState() == GObject::State::STOP) {
			continue;
		}
		Vec2 characterPos = convertToNodeSpace(m_Character->getPosition());
		Vec2 dir = characterPos - point->getPosition();
		float dirLength = dir.length();
		float charRadius = m_Character->getRadius();
		float distance = m_Character->getRadius() * 1.5f;
		if (dirLength <= distance) {
			addChild(Particle::createNew()->particleOneTime(point->getPosition(), RESOURCE(R_R_particle_star_plist)));
			point->stop();
			m_Base->addScore(point->getPoint());
			return true;
		}
	}
	return false;
}

void PointFragment::updateAliveLimit()
{
	m_AliveLimit.origin.y = m_Camera->getPositionY();
}

void PointFragment::filterVisibleObjects()
{
	Rect visibleRect{ convertToNodeSpace(m_Camera->getPosition()), m_VisibleSize };
	for (auto pair : m_Planets) {
		CPlanet * planet = pair.second;
		if (visibleRect.intersectsRect(planet->getBoundingBox()) && planet->getState() == GObject::State::WORK) {
			planet->setVisible(true);
		}
		else {
			planet->setVisible(false);
		}
	}
}

void PointFragment::rotateCharAroundPlanet(float duration)
{
	if (!m_CurrentPlanet) {
		return;
	}
	Vec2 dir = convertToNodeSpace(m_Character->getPosition()) - m_CurrentPlanet->getPosition();
	//rotate character
	dir = mtl::rotateZ(m_CharacterAgSpeed * duration, dir);
	m_Character->setPosition(convertToWorldSpace(dir + m_CurrentPlanet->getPosition()));
	//move camera when out of view
	Vec2 planetPosInCamera = m_Camera->worldToCamera(convertToWorldSpace(m_CurrentPlanet->getPosition()));
	if (!m_CameraLimit.containsPoint(planetPosInCamera)) {
		Vec2 direction = planetPosInCamera - Vec2(m_CameraLimit.getMidX(), m_CameraLimit.getMidY());
		m_Camera->setVelocity(direction.getNormalized() * m_WinSize.width);
	}
	else {
		m_Camera->stopMove();
	}
}

bool PointFragment::isMainCharOutOfCamera()
{
	return !m_AliveLimit.containsPoint(m_Character->getPosition()) && !m_CurrentPlanet;
}

void PointFragment::updateAll(float duration)
{
	Vector<Node*> worldChildren = getChildren();
	for (Node* worldChild : worldChildren) {
		worldChild->update(duration);
	}
}

void PointFragment::onEnter()
{
	Node::onEnter();
	initEventListeners();
}

void PointFragment::update(float duration)
{
	if (m_CurrentState == IFragment::State::PAUSE || m_CurrentState == IFragment::State::STOP) {
		return;
	}
	updateAliveLimit();
	filterVisibleObjects();
	rotateCharAroundPlanet(duration);
	findCollisionPlanet();
	findColiisionPoint();
	if (isMainCharOutOfCamera()) {
		m_Base->setGameOver();
		pause();
	}
	updateAll(duration);
}

void PointFragment::setBase(IFragmentBase * base)
{
	if (!base) {
		return;
	}
	m_Base = base;
	reInit();
}

cocos2d::Size PointFragment::getSize()
{
	return m_MapSize;
}

IFragment::State PointFragment::getState()
{
	return m_CurrentState;
}

cocos2d::Rect PointFragment::getRect()
{
	return cocos2d::Rect(getPosition(), m_MapSize);
}
