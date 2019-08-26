#include "GameScene.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"
#include "tool/Tool.hpp"
#include "3D/SmallBoss3D.hpp"
#include "3D/GAnimation3DCache.hpp"
#include "manager/ResourceManager.hpp"
#include "fragment/PointFragment.hpp"
#include "UI/GameSceneUI.h"
#include "SceneTutorial.h"
#include <cmath>
#include <string>

USING_NS_CC;

Scene* GameScene::createScene()
{
	return GameScene::create();
}

static int oldAgSpeed;

/*Implementing IFragmentBase*/
float GameScene::getScore()
{
	return m_Score;
}

void GameScene::setScore(float score)
{
	m_Score = score;
}

void GameScene::addScore(float amount)
{
	m_Score += int(amount);
	m_UI->updateScoreUI(m_Score, m_UserData->getHighScore(), m_Combo);
}

GCamera * GameScene::getCamera()
{
	return m_Camera;
}

GObject * GameScene::getCharacter()
{
	return m_Character;
}

void GameScene::addUI(cocos2d::Node * ui)
{
	addChild(ui, 1);
}
/* End implementing*/

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in LoadingScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	//Visible size, origin, win size
	m_UserData = UserData::getInstance();
	m_VisibleSize = _director->getVisibleSize();
	m_Origin = _director->getVisibleOrigin();
	m_WinSize = _director->getWinSize();

	//save data from config
	ResourceManager* resManager = ResourceManager::getInstance();
	m_MainToPeekTime = resManager->getConfig(R_mainToPeekTime);
	m_MainToPeekWinMulitply = resManager->getConfig(R_mainToPeekWinMultiply);
	m_MainSelfRotationSpeed = resManager->getConfig(R_mainSelfRotationSpeed);
	m_MainRotationSpeed = resManager->getConfig(R_mainRotationSpeed);

	// Khoi tao mUI
	//m_UI = GameSceneUI::createNew();
	initUI();
	initWorld();
	return true;
}
//world objects like planet, meteorite, main
void GameScene::initWorld()
{
	/****init object*****/

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
	m_MainSelfRotationSpeed = resManager->getConfig(R_mainSelfRotationSpeed);
	oldAgSpeed = m_MainRotationSpeed = resManager->getConfig(R_mainRotationSpeed);

	//World node
	world = Node::create();
	//ambient, dir light
	DirectionLight* dirLight = DirectionLight::create({ -1, -1, -1 }, Color3B::WHITE);

	AmbientLight* ambient = AmbientLight::create({ 100, 100, 100 });
	world->addChild(dirLight);
	world->addChild(ambient);
	//Camera
	m_Camera = GCamera::createOrthographic(m_WinSize.width, m_WinSize.height, 0, 1000);
	m_Camera->setPositionZ(500);
	m_Camera->setCameraFlag(CameraFlag::USER1);
	world->addChild(m_Camera);

	//*-* Backgroud
	m_Background = BackGround::createNew();
	m_Background->m_Camera = m_Camera;
	m_Background->setGlobalZOrder(-1);
	m_Background->LineLeft->setPositionX(m_AliveLimit.getMinX());
	m_Background->LineRight->setPositionX(m_AliveLimit.getMaxX());
	world->addChild(m_Background, -1);
	//*-* Cplanet create 6

	float width;
	float height = m_Origin.y - m_VisibleSize.width*0.5;
	for (int i = 0; i < R_numPlanet; i++)
	{
		CPlanet *planet = CPlanet::createNew();
		planet->setAnim(CPlanet::NORMAL_ANIM1, FRAMES(R_A_PLANET_5), 30);
		planet->setAnim(CPlanet::NORMAL_ANIM2, FRAMES(R_A_PLANET_6), 30);
		planet->setAnim(CPlanet::NORMAL_ANIM3, FRAMES(R_A_PLANET_7), 30);
		planet->setAnim(CPlanet::NORMAL_ANIM4, FRAMES(R_A_PLANET_8), 30);
		planet->setAnim(CPlanet::NORMAL_ANIM5, FRAMES(R_A_PLANET_4), 30);
		planet->reInit();
		planet->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });
		width = random(m_Origin.x + m_VisibleSize.width / 6 + planet->getContentSize().width,
			m_Origin.x + m_VisibleSize.width - planet->getContentSize().width);
		height += m_VisibleSize.width*0.85 ;
		planet->setPosition(width, height);
		planet->setAgSpeed(0);
		planet->setCamera(m_Camera);
		planet->setNumber(i);
		planet->work();
		world->addChild(planet, 1);
		m_Planets.push_back(planet);
		m_GObjects.push_back(planet);
		m_PlanetCount++;
	}
	//Character or main
	m_Character = CCharacter::createNew();
	m_Character->setAnim(CCharacter::NORMAL_ANIM, { "Cf3.png" }, 0);
	m_Character->setPosition(m_Planets.at(0)->getPosition() + Vec2(1, 1));
	m_Character->setAnchorPoint(Vec2(0.5f, 0.5f));
	m_Character->setAgSpeed(m_MainSelfRotationSpeed);
	m_Character->reInit();
	m_Character->setBlendFunc({ GL_SRC_ALPHA,GL_ONE });
	m_Character->work();
	world->addChild(m_Character,1);
	m_GObjects.push_back(m_Character);

	m_Character->setAnim(CCharacter::NORMAL_ANIM, FRAMES(R_A_ITEM_1), 30);

	for (int i = 0; i < 5; i++) {
		Meteorite* meteorite = Meteorite::createNew();
		meteorite->setAnim(Meteorite::NORMAL_ANIM,FRAMES(R_A_METEO_1) , 20);
		meteorite->setCamera(m_Camera);
		meteorite->reInit();
		meteorite->stop();
		world->addChild(meteorite,1);
		m_Meteorites.push_back(meteorite);
		m_GObjects.push_back(meteorite);
	}
	
	for (int i = 0; i < 5; i++) {
		CItemSpeed* item = CItemSpeed::createNew();
		item->setAnim(Meteorite::NORMAL_ANIM, FRAMES(R_A_ITEM_1), 20);
		item->reInit();
		item->setVisible(false);
		item->setCamera(m_Camera);
		world->addChild(item, 1);
		m_items.push_back(item);
		m_GObjects.push_back(item);
	}

	//Warnings
	m_MeteWarning = MeteoriteWarning::createNew();
	m_MeteWarning->initWithFile(RESOURCE(R_R_IMG_warning3_png));
	m_MeteWarning->stop();
	world->addChild(m_MeteWarning, 10);
	m_GObjects.push_back(m_MeteWarning);
	//
	m_Arrow = CArrow::createNew();
	m_Arrow->initWithFile(RESOURCE(R_R_IMG_Arrow_png));
	m_Arrow->setScale(0.25f);
	m_Arrow->setPosition({300, 300});
	m_Arrow->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });
	m_Arrow->setAnchorPoint({ 0.5f,0 });
	//m_Arrow->reInit();
	m_Arrow->stop();
	world->addChild(m_Arrow);
	m_GObjects.push_back(m_Arrow);

	//alert arrow
	m_AlertArrow = CArrow::createNew();
	m_AlertArrow->initWithFile(RESOURCE(R_R_IMG_Arrow_png));
	m_AlertArrow->setScale(0.25f);
	m_AlertArrow->setPosition({ 300, 300 });
	//m_Arrow->reInit();
	m_AlertArrow->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });
	m_AlertArrow->setAnchorPoint({ 0.5f,0 });
	m_AlertArrow->stop();
	world->addChild(m_AlertArrow);
	m_GObjects.push_back(m_AlertArrow);

	//add world to scene
	world->setCameraMask((unsigned short)CameraFlag::USER1);
	addChild(world);
	m_HaveUpdateLayout = true;
}

void GameScene::initUI()
{
	//Replay button
	auto buttonReplay = MenuItemImage::create(RESOURCE(R_R_IMG_Play_BTN_png), RESOURCE(R_R_IMG_Play_BTN1_png), CC_CALLBACK_1(GameScene::menuReplayCallback, this));
	menuReplay = Menu::create(buttonReplay,NULL);
	menuReplay->setVisible(false);
	menuReplay->retain();

	// Them class UI
	m_UI = GameSceneUI::createNew();
	m_UI->reInit(m_Score, m_UserData->getHighScore());
	addChild(m_UI, 1);
	//------------------------------

	//Sound
	auto sound = SimpleAudioEngine::getInstance();
	sound->preloadEffect(RESOURCE(R_R_S_sfx_point_mp3));
	sound->preloadEffect(RESOURCE(R_R_S_Jump_mp3));
	sound->playBackgroundMusic(RESOURCE(R_R_S_HypnoticPuzzle_mp3), true);
	sound->setBackgroundMusicVolume(UserData::getInstance()->getAudio());
	sound->setEffectsVolume(UserData::getInstance()->getAudio());

	// Tutorial
	if (isTutorial == true) {
		m_DialBox = Sprite::create(RESOURCE(R_R_IMG_dial_png));
		m_Message1 = Label::createWithTTF("welcome back!\nclick to the screen to jump to\nother planets\ntry to stay away from asteroids\nthere will be warnings for you", RESOURCE(R_R_f_ethno_ttf), 21);
		auto imageSize = m_DialBox->getContentSize();
		if (m_Message1 == nullptr)
		{
			problemLoading(RESOURCE(R_R_f_Marker_Felt_ttf));
		}
		else
		{
			m_Message1->setPosition(imageSize.width / 25, imageSize.height - imageSize.height / 5);
			m_Message1->setColor(Color3B::YELLOW);
			m_Message1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
			m_Message1->setOpacity(0);
			// add the label as a child to this layer
			m_DialBox->addChild(m_Message1);
		}
		m_DialBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		Vec2 dgPos = Vec2(m_Origin.x + m_VisibleSize.width / 2, m_Origin.y + m_VisibleSize.height / 8);
		m_DialBox->setPosition(dgPos);
		addChild(m_DialBox, 10);
	}
}

void GameScene::initEventListeners()
{
	EventListenerTouchOneByOne *touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	EventListenerKeyboard *keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

	EventListenerCustom *gameTutorial = EventListenerCustom::create("Tutorial", [this](EventCustom *event) {
		_director->pushScene(TransitionFade::create(1, TutorialScene::createScene()));
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(gameTutorial, this);
}

void GameScene::update(float duration)
{
	if (m_PlanetCount > 100 && flag) {
		m_HaveUpdateLayout = false;
		fragment = PointFragment::createNew(RESOURCE(R_R_map2_tmx));
		fragment->setBase(this);
		fragment->setPosition({ 0, m_Planets.back()->getPositionY() + m_VisibleSize.width / 2 });
		world->addChild(fragment, 1);
		fragment->setCameraMask(world->getCameraMask());
		fragment->start();
		fragment->resume();
		flag = false;
	}
	if (m_CurrentState == State::STOP) {
		return;
	}
	filterVisibleObjects();
	updateAliveLimit();
	if (!m_StartGame) //waiting for first tap
	{
		type(duration);
		runEasyStage(duration);
	}

	if (m_StartGame) //GamePlay
	{
		if (m_Score <= 300) {
			float oldSpeed = m_MainRotationSpeed;
			m_MainRotationSpeed = oldAgSpeed + 10 * (int)(m_Score / 25);
			if (m_MainRotationSpeed - oldSpeed > 10) {
				//message
			}
		}
		if (m_Score > 30) {
			runMeteoriteStage(duration);
		}
		else {
			runEasyStage(duration);
		}
	}

	if (m_CurrentPlanet && m_CurrentPlanet->m_status == P_RED)
	{
		m_Character->m_timeLife -= duration;
	}
	if (isGameOver() && !m_GameOver)
	{
		setGameOver();
		return;
	}
	updateAll(duration);
}

bool GameScene::isGameOver()
{
	if (m_Character->m_timeLife < 0 && !m_GameOver)
		return true;
	if (m_CurrentPlanet && m_CurrentPlanet->collision == 0 && m_CurrentPlanet->m_status == P_YELLOW)
		return true;
	return false;
}

void GameScene::setGameOver()
{
	//Thu UI
	auto m_ComboLabel = m_UI->getComboLabel();
	//--------------------------------
	m_GameOver = true;
	// Thu UI
	m_UI->updateScoreUI(m_Score, m_UserData->getHighScore(), m_Combo);
	//--------------------
	/* Tat de thu UI
	updateScoreUI();
	*/
	m_ComboLabel->setVisible(false);
	m_Character->setVisible(false);
	m_Camera->stopMove();
	world->addChild(Particle::createNew()->particleCharDie(m_Character), 10);
	cocos2d::Device::vibrate(0.4f);

	//High score panel
	auto panel = Sprite::create(RESOURCE(R_R_IMG_highScore2_png));
	panel->setOpacity(0);
	panel->runAction(Sequence::create(DelayTime::create(1), FadeIn::create(1), nullptr));
	panel->setPosition(m_Origin.x + m_VisibleSize.width / 2, m_Origin.y + m_VisibleSize.height / 2);
	auto size = panel->getContentSize();

	auto scoreLabel = Label::createWithTTF(std::to_string(m_Score), RESOURCE(R_R_f_ethno_ttf), 50);
	scoreLabel->setPosition(Vec2(size.width * 0.73, size.height * 0.681 ));
	scoreLabel->setColor(Color3B::WHITE);
	scoreLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	scoreLabel->setOpacity(0);
	scoreLabel->runAction(Sequence::create(DelayTime::create(1), FadeIn::create(1), nullptr));
	panel->addChild(scoreLabel);
	
	auto bestLabel = Label::createWithTTF(std::to_string(m_UserData->getHighScore()), RESOURCE(R_R_f_ethno_ttf), 50);
	bestLabel->setColor(Color3B::WHITE);
	bestLabel->setPosition(Vec2(size.width * 0.73, size.height * 0.46));
	bestLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	bestLabel->setOpacity(0);
	bestLabel->runAction(Sequence::create(DelayTime::create(1), FadeIn::create(1), nullptr));
	panel->addChild(bestLabel);

	menuReplay->setVisible(true);
	menuReplay->setPosition(Vec2(size.width / 2 , size.height * 0.2));
	menuReplay->setOpacity(0);
	menuReplay->runAction(Sequence::create(DelayTime::create(1), FadeIn::create(1), nullptr));
	panel->addChild(menuReplay);
	this->addChild(panel,10);
}

bool GameScene::isMainCharOutOfCamera()
{
	return !m_AliveLimit.containsPoint(m_Character->getPosition()) && !m_CurrentPlanet;
}

bool GameScene::findCollisionPlanet()
{
	//find collision planet and save it
	//increase score if passing planets
	for (CPlanet *planet : m_Planets) {
		Vec2 dir = m_Character->getPosition() - planet->getPosition();
		if (planet->getBoundingBox().containsPoint(m_Character->getPosition()) && planet->m_collision) {
			float dirLength = dir.length();
			float charRadius = m_Character->getRadius();
			float distance = planet->getRadius() + charRadius + charRadius / 4;
			if (dirLength <= distance) {
				for (CPlanet *_planet : m_Planets)
				{
					_planet->setMove(true);
					_planet->m_collision = true;
				}

				planet->setOpsion();
				if (planet->m_status == P_YELLOW && planet->collision == 0)
				{
					planet->stop();
					setGameOver();
				}
				//----
				if (getChildByName("Speed"))
				{
					getChildByName("Speed")->stopAllActions();
					removeChild(getChildByName("Speed"));
				};
				addChild(Particle::createNew()->particleOneTime(planet->getPosition(), RESOURCE(R_R_particle_collision_plist)));
				m_Character->m_timeLife = 5;
				m_Character->covertStatus(planet->m_status);
				m_Character->m_timeStatus = 0;
				//find rotation direction
				Vec2 mainVelocity = m_Character->getVelocity();
				Vec3 mainDir3D{ mainVelocity.x, mainVelocity.y, 0 };
				Vec3 toPlanetDir3D{ dir.x, dir.y, 0 };
				Vec3 normal;
				Vec3::cross(toPlanetDir3D, mainDir3D, &normal);
				if (normal.z >= 0) {
					m_CharacterAgSpeed = m_MainRotationSpeed;
				}
				else {
					m_CharacterAgSpeed = -m_MainRotationSpeed;
				}
				dir = dir * distance / dirLength;
				m_Character->stopMove();
				m_Character->setPosition(dir + planet->getPosition());

				m_Camera->stopMove();
				m_CurrentPlanet = planet;
				//---perfect jump
				m_Combo = planet->getCombo(m_Combo);
				m_UI->upScore(m_Score, m_Combo, m_UserData);
				//--

				return true;
			}
		}
		if (m_Character->m_status == C_GREEN && planet->getScore() > 0 && dir.y > 0)
		{
			m_Combo = planet->getCombo(m_Combo);
			m_UI->upScore(m_Score, m_Combo, m_UserData);
		}
	}
	return false;
}

bool GameScene::isCollideWithMeteorites()
{
	//check collision
	for (Meteorite *met : m_Meteorites) {
		if (met->getState() == GObject::State::WORK) {
			for (CPlanet *planet : m_Planets) {
				if (met->getBoundingBox().intersectsRect(planet->getBoundingBox())) {
					Vec2 dir = planet->getPosition() - met->getPosition();
					float dirLength = dir.length();
					float metRadius = met->getContentSize().height / 1.5;
					if (dirLength <= metRadius) {		
						world->addChild(Particle::createNew()->particlePlanet(planet));
						met->stop();
						return true;
					}
				}
			}
			if (met->getBoundingBox().intersectsRect(m_Character->getBoundingBox())) {
				Vec2 dir = m_Character->getPosition() - met->getPosition();
				float dirLength = dir.length();
				float metRadius = met->getContentSize().height / 1.5;
				if (dirLength <= metRadius) {
					cocos2d::Device::vibrate(0.05f);
					met->stop();
					if (m_Character->m_status != C_LIGHTING)
						setGameOver();
					else
					{
						addChild(Particle::createNew()->particleOneTime(met->getPosition(), RESOURCE(R_R_particle_star_plist)));
					}
					return true;
				}
			}
		}
	}
	return false;
}

void GameScene::rotateCharAroundPlanet(float duration)
{
	Vec2 dir = m_Character->getPosition() - m_CurrentPlanet->getPosition();
	//rotate character
	m_CharacterRotationTime += duration;
	if (m_Character->m_status == C_BLUE)
	{
		m_CharacterAgSpeed = m_CharacterAgSpeed/ std::abs(m_CharacterAgSpeed) * BLU_SPEED;
	}
	if (m_Character->m_status == C_LIGHTING)
	{
		m_CharacterAgSpeed = m_CharacterAgSpeed / std::abs(m_CharacterAgSpeed) * LIGHTING_SPEED;
	}
	dir = mtl::rotateZ(m_CharacterAgSpeed * duration, dir);
	m_Character->setPosition(dir + m_CurrentPlanet->getPosition());
	m_Character->setVelocity(m_CurrentPlanet->getVelocity());
	m_Character->setAcceleration(m_CurrentPlanet->getAcceleration());
	if (m_Score < 10) {
		dir.normalize();
		setArrow(m_Arrow, m_Character->getPosition() + dir * m_Character->getRadius(), dir);
	}
	else {
		m_Arrow->stop();
	}
	//move camera when out of view
	Vec2 planetPosInCamera = m_Camera->worldToCamera(m_CurrentPlanet->getPosition());
	Vec2 cameraPos = m_Camera->getPosition();
	if (!m_CameraLimit.containsPoint(planetPosInCamera)) {
		Vec2 direction = planetPosInCamera - Vec2(m_CameraLimit.getMidX(), m_CameraLimit.getMidY());
		m_Camera->setVelocity(direction.getNormalized() * m_WinSize.width);
	}
	else {
		m_Camera->setVelocity(m_CurrentPlanet->getVelocity());
		m_Camera->setAcceleration(m_CurrentPlanet->getAcceleration());
	}
}

//Generating planets: reuse planets, only change their positons
void GameScene::generatePlanets()
{
	CPlanet * firstPlanet = m_Planets.front();
	bool isChange = firstPlanet->rePos(m_Camera->worldToCamera(m_Planets.back()->getPosition()));
	if (isChange) {
		m_Planets.pop_front();
		m_Planets.push_back(firstPlanet);
		m_PlanetCount++;
	}
}

void GameScene::calculateMeteoritesVelocity(Meteorite * met)
{
	Vec2 moveDir = m_CurrentPlanet->getPosition() - met->getPosition();
	float totalDistance = moveDir.length();
	float oneRoundTime = std::abs(360 * m_NumRoundsForComing / m_CharacterAgSpeed);
	m_DistanceWhenComing = m_CurrentPlanet->getRadius() * 3;
	moveDir.normalize();
	met->setVelocity(moveDir * (totalDistance - m_DistanceWhenComing) / oneRoundTime);
}

//Generating meteorites: reuse meteorites, only change their positions
void GameScene::generateMeteorites()
{
	auto m_VisibleSize = _director->getVisibleSize();
	if (m_CurrentPlanet) {
		Meteorite* met = m_Meteorites.front();
		m_Meteorites.pop_front();
		m_Meteorites.push_back(met);
		met->rePos();
		met->work();

		Vec2 A = met->getPosition();
		Vec2 B = m_CurrentPlanet->getPosition();
		Vec3 dir{ B.x - A.x, B.y - A.y, 0 }; //Vector dir
		Vec3 pivot{ 1, 0 , 0 };				//Vector xoay
		pivot.normalize();
		dir.normalize();
		Quaternion quat = mtl::getQuaternion(pivot, dir); //Goc xoay
		met->setRotationQuat(quat);
		calculateMeteoritesVelocity(met);
	}
}

void GameScene::generateMeteoriteWarning() {
	float meteDistanceFromPlanet = m_CurrentPlanet->getRadius() * 3.5f;
	Vec2 planetToMeteoriteDir = m_Meteorites.back()->getPosition() - m_CurrentPlanet->getPosition();
	planetToMeteoriteDir.normalize();
	m_MeteWarning->setPosition(m_CurrentPlanet->getPosition() + planetToMeteoriteDir * meteDistanceFromPlanet);
	m_MeteWarning->work();
	m_MeteWarning->setOpacity(0);
	m_MeteWarning->stopAllActions();
	setArrow(m_AlertArrow,
		m_MeteWarning->getPosition() + (planetToMeteoriteDir * m_MeteWarning->getContentSize().width * 2),
		-planetToMeteoriteDir);
	m_MeteWarning->runAction(Sequence::create(FadeIn::create(0.5), FadeOut::create(0.5),
		FadeIn::create(0.25), FadeOut::create(0.25), CallFunc::create(CC_CALLBACK_0(GameScene::stopAlertArrow, this)), NULL));
}


//Update Alive limit when camera move
void GameScene::updateAliveLimit()
{

	m_AliveLimit.origin.y = m_Camera->getPositionY();
}

bool GameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event * event)
{
	onFiringCharacter();
	return true;
}

void GameScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event * event)
{
	if (key == cocos2d::EventKeyboard::KeyCode::KEY_SPACE) {
		onFiringCharacter();
	}
}

void GameScene::type(float dt)
{
	if (!isTutorial){
		return;
	}
	auto a = m_Message1->getString();
	if (i < m_Message1->getStringLength())
	{
		if (a[i] == ' ' || a[i] == '\n') {
			i++;
		}
		auto letter = m_Message1->getLetter(i);
		letter->setColor(Color3B::YELLOW);
		letter->setOpacity(255);
		i++;
	}
	else {
		i = 0;
		isTutorial = false;
	}
}

void GameScene::updateAll(float duration)
{
	m_Camera->update(duration);
	m_Background->update(duration);
	for (GObject * object : m_GObjects) {
		object->update(duration);
	}
	if (fragment) {
		fragment->update(duration);
	}
}

void GameScene::onFiringCharacter()
{
	if (m_CurrentState != State::WORK || m_GameOver ||
		!m_CurrentPlanet) return;

	if (!m_StartGame) { m_StartGame = true; }
	if (m_DialBox != nullptr)
		this->removeChild(m_DialBox);
	if (m_Score > 0 || m_Combo > 0) {
		SimpleAudioEngine::getInstance()->playEffect(RESOURCE(R_R_S_Jump_mp3));
	}
	Vec2 dir = m_Character->getPosition() - m_CurrentPlanet->getPosition();
	dir.normalize();
	float s = m_WinSize.width * m_MainToPeekWinMulitply;
	float accelerationLength = -2 * s / (m_MainToPeekTime * m_MainToPeekTime);
	float velocityLength = -accelerationLength * m_MainToPeekTime;
	Vec2 velocity = dir * velocityLength;
	Vec2 acceleration = dir * accelerationLength;

	m_CurrentPlanet->setMove(false);
	if (m_Character->m_status == C_GREEN && m_Character->m_timeStatus > 2)
	{
		//Vô hiệu hóa va chạm chỉ bật va chạm của planet đích
		for (CPlanet *planet : m_Planets )
		{
			planet->m_collision = false;
		}
		//Tìm planet đích qua number đánh dấu đầu khởi tạo
		for (CPlanet *planet : m_Planets)
		{
			if (planet->getNumber() == (m_CurrentPlanet->getNumber()+5)%R_numPlanet)
			{
				dir = -m_Character->getPosition() + planet->getPosition();
				planet->m_collision = true;
				planet->setMove(false);
				break;
			}
		}

		//Chuẩn hóa và tính toán hướng bay
		dir.normalize();
		s = m_WinSize.width * m_MainToPeekWinMulitply;
		accelerationLength = -2 * s / (m_MainToPeekTime * m_MainToPeekTime);
		velocityLength = -accelerationLength * m_MainToPeekTime;
		velocity = dir * velocityLength;
		acceleration = dir * accelerationLength;

		velocity *= 2.0f;
		acceleration *= 0.8f;
		addChild(Particle::createNew()->particleOneTime({ 0,0 }, RESOURCE(R_R_particle_speed_plist), "Speed", 1.0f, 2, -1));
	}

	if (m_Character->m_status == C_LIGHTING)
	{
		velocity *= 2.0f;
		acceleration *= 4;
	}

	m_Character->setVelocity(velocity);
	m_Character->setAcceleration(acceleration);
	m_Camera->setVelocity(velocity);
	m_Camera->setAcceleration(acceleration);

	m_CurrentPlanet = nullptr;
	m_CharacterRotationTime = 0;
}


cocos2d::Rect GameScene::getVisibleRect()
{
	return cocos2d::Rect(m_Camera->getPosition(), m_VisibleSize);
}

void GameScene::runFragment()
{
}


void GameScene::start()
{
	m_CurrentState = State::START;
}

void GameScene::start(cocos2d::Vec2 pos)
{
	m_CurrentState = State::START;
	for (CPlanet * planet : m_Planets)
	{
		float width = random(m_Origin.x + m_VisibleSize.width / 6 + planet->getContentSize().width,
			m_Origin.x + m_VisibleSize.width - planet->getContentSize().width);
		planet->setPosition(width, pos.y);
		planet->work();
		pos.y += m_VisibleSize.width*0.85;
	}
}

void GameScene::work()
{
	m_CurrentState = State::WORK;
}

void GameScene::stop()
{
	m_CurrentState = State::STOP;
}

void GameScene::onEnter()
{
	Scene::onEnter();
	initEventListeners();
	start();
	work();
	scheduleUpdate();
}

void GameScene::addFragment(IFragment * fragment, int numPlanets, bool haveMeteorites)
{
	FragmentSlot slot;
	slot.m_NumPlanets = numPlanets;
	slot.m_HaveMeteorites = haveMeteorites;
	slot.m_Fragment = fragment;
	//m_FragmentSlots.push_back(slot);
}

void GameScene::menuSceneCallback(Ref* pSender)
{
	_director->pushScene(MainMenuScene::create());
}

void GameScene::menuReplayCallback(Ref* pSender)
{
	_director->replaceScene(TransitionFade::create(0.5,GameScene::createScene()));
}

void GameScene::runEasyStage(float duration)
{
	if (m_GameOver) {
		return;
	}
	if (m_CurrentPlanet) {
		m_temp = m_CurrentPlanet;
		rotateCharAroundPlanet(duration);
	}
	else {
		if (findCollisionPlanet() ) {
			if (m_CurrentPlanet != m_temp && m_Score != 0) {
				SimpleAudioEngine::getInstance()->playEffect(RESOURCE(R_R_S_sfx_point_mp3));
			}
			else if (m_CurrentPlanet == m_temp) {
				SimpleAudioEngine::getInstance()->playEffect(RESOURCE(R_R_S_sfx_hit_mp3));
			}
		}
	}
	updateLayout(false);
	if ((isMainCharOutOfCamera() )) {
		setGameOver();
		return;
	}
}

void GameScene::runMeteoriteStage(float duration)
{
	if (m_GameOver) {
		return;
	}
	bool haveNewCurrentPlanet = false;
	if (m_CurrentPlanet) {
		m_temp = m_CurrentPlanet;
		rotateCharAroundPlanet(duration);
	}
	else {
		haveNewCurrentPlanet = findCollisionPlanet();
	}
	if (haveNewCurrentPlanet) {
		if (m_Score > 0 || m_Combo > 0) {
			if (m_temp != m_CurrentPlanet) {
				SimpleAudioEngine::getInstance()->playEffect(RESOURCE(R_R_S_sfx_point_mp3));
			}
			else if (m_CurrentPlanet == m_temp) {
				SimpleAudioEngine::getInstance()->playEffect(RESOURCE(R_R_S_sfx_hit_mp3));
			}
		}
	}
	isCollideWithMeteorites();
	if (isMainCharOutOfCamera()) {
		setGameOver();
		return;
	}
	updateLayout(haveNewCurrentPlanet);
	
}

void GameScene::goSceneTutorial()
{
	_director->pushScene(TutorialScene::createScene());
}

void GameScene::setArrow(CArrow * arrow, const cocos2d::Vec2 & pos, const cocos2d::Vec2 & dir)
{
	if (arrow->getState() != GObject::State::WORK) {
		arrow->work();
	}
	arrow->setPosition(pos);
	arrow->setRotationQuat(mtl::getQuaternion({ 0, 1, 0 }, {dir.x, dir.y, 0}));
}

void GameScene::stopAlertArrow()
{
	m_AlertArrow->stop();
}

void GameScene::updateLayout(bool haveMeteorites)
{
	if (!m_HaveUpdateLayout) {
		return;
	}
	generatePlanets();
	if (haveMeteorites) {
		//generating meteorites
		generateMeteorites();
		//generating warning
		generateMeteoriteWarning();
	}
}

void GameScene::filterVisibleObjects()
{
	Rect visibleRect{ m_Camera->getPosition(), m_VisibleSize };
	for (CPlanet * planet : m_Planets) {
		if (visibleRect.intersectsRect(planet->getBoundingBox()) && planet->getState() != GObject::State::STOP) {
			planet->setVisible(true);
		}
		else {
			planet->setVisible(false);
		}
	}

	for (Meteorite * meteorite : m_Meteorites) {
		if (visibleRect.intersectsRect(meteorite->getBoundingBox()) && meteorite->getState() != GObject::State::STOP) {
			meteorite->setVisible(true);
		}
		else {
			meteorite->setVisible(false);
		}
	}
}
