#include "SceneTutorial.h"
#include "MainMenuScene.h"
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "tool/Tool.hpp"
#include "3D/SmallBoss3D.hpp"
#include "3D/GAnimation3DCache.hpp"
#include "manager/ResourceManager.hpp"
#include <cmath>

USING_NS_CC;

Scene* TutorialScene::createScene()
{
	return TutorialScene::create();
}


// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in LoadingScene.cpp\n");
}

// on "init" you need to initialize your instance
bool TutorialScene::init()
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

	//load data from config
	ResourceManager* resManager = ResourceManager::getInstance();
	m_MainToPeekTime = resManager->getConfig(R_mainToPeekTime);
	m_MainToPeekWinMulitply = resManager->getConfig(R_mainToPeekWinMultiply);
	m_MainSelfRotationSpeed = resManager->getConfig(R_mainSelfRotationSpeed);
	m_MainRotationSpeed = resManager->getConfig(R_mainRotationSpeed);

	// Back button
	auto labelBack = Label::createWithTTF("back", RESOURCE(R_R_f_spac_ttf), 50);
	labelBack->setColor(Color3B::WHITE);
	auto buttonBack = new MenuItemLabel();
	buttonBack->initWithLabel(labelBack, CC_CALLBACK_1(TutorialScene::menuSceneCallback, this));
	auto menuBack = Menu::create(buttonBack, NULL);
	menuBack->setPosition(m_Origin.x + m_VisibleSize.width - buttonBack->getContentSize().width / 2, m_Origin.y + buttonBack->getContentSize().height / 2);
	menuBack->setVisible(true);
	this->addChild(menuBack, 1);

	//*-*
	initSceneObjects();
	world = initWorldObjects();
	addChild(world);
	initEventListener();
	this->scheduleUpdate();
	m_Background->scheduleUpdate();
	
	// Them am thanh
	auto sound = SimpleAudioEngine::getInstance();
	sound->preloadEffect(RESOURCE(R_R_S_sfx_point_mp3));
	sound->preloadEffect(RESOURCE(R_R_S_Jump_mp3));
	sound->playBackgroundMusic(RESOURCE(R_R_S_HypnoticPuzzle_mp3), true);
	sound->setBackgroundMusicVolume(0.5);
	return true;
}

//world objects like planet, meteorite, main
Node* TutorialScene::initWorldObjects()
{
	/****init object*****/
	//World node
	Node* node = Node::create();

	//ambient, dir light
	DirectionLight* dirLight = DirectionLight::create({ -1, -1, -1 }, Color3B::WHITE);

	AmbientLight* ambient = AmbientLight::create({ 100, 100, 100 });
	node->addChild(dirLight);
	node->addChild(ambient);
	//Camera
	m_Camera = GCamera::createOrthographic(m_WinSize.width, m_WinSize.height, 0, 1000);
	m_Camera->setPositionZ(500);
	m_Camera->setCameraFlag(CameraFlag::USER1);
	node->addChild(m_Camera, 10);

	//*-* Backgroud
	m_Background = BackGround::createNew();
	m_Background->m_Camera = m_Camera;
	m_Background->setGlobalZOrder(-1);
	node->addChild(m_Background, -1);
	//*-* Cplanet create 6

	float width;
	float height = m_Origin.y - m_VisibleSize.width*0.4f;
	for (int i = 0; i < 6; i++)
	{
		CPlanet *_planet = CPlanet::createNew();

		_planet->setAnim(CPlanet::NORMAL_ANIM1, FRAMES(R_A_PLANET_5), 30);
		_planet->setAnim(CPlanet::NORMAL_ANIM2, FRAMES(R_A_PLANET_6), 30);
		_planet->setAnim(CPlanet::NORMAL_ANIM3, FRAMES(R_A_PLANET_7), 30);
		_planet->setAnim(CPlanet::NORMAL_ANIM4, FRAMES(R_A_PLANET_8), 30);

		_planet->reInit();
		_planet->setBlendFunc({ GL_SRC_ALPHA, GL_ONE });
		if (0 == i || 1 == i) {
			width = m_Origin.x + m_VisibleSize.width / 2;
		}
		else if(2 == i)
			width = m_Origin.x + m_VisibleSize.width / 4;
		else {
			width = random(m_Origin.x + m_VisibleSize.width / 6 + _planet->getContentSize().width, 
				m_Origin.x + m_VisibleSize.width - _planet->getContentSize().width);

		}
		height += m_VisibleSize.width;
		_planet->setPosition(width, height);
		_planet->setAgSpeed(0);
		_planet->setCamera(m_Camera);
		_planet->work();
		node->addChild(_planet, 1);
		m_Planets.pushBack(_planet);
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
	node->addChild(m_Character, 1);

	for (int i = 0; i < 1; i++) {
		Meteorite* meteorite = Meteorite::createNew();
		meteorite->setAnim(Meteorite::NORMAL_ANIM, FRAMES(R_A_METEO_1), 20);
		meteorite->setCamera(m_Camera);
		m_Meteorites.push_back(meteorite);
		meteorite->reInit();
		meteorite->stop();
		node->addChild(meteorite, 1);
	}

	//Warnings
	m_MeteWarning = MeteoriteWarning::createNew();
	m_MeteWarning->initWithFile(RESOURCE(R_R_IMG_warning3_png));
	node->addChild(m_MeteWarning, 10);

	

	node->setCameraMask((unsigned short)CameraFlag::USER1);
	//add world to scene

	m_Planets.at(3)->setPositionX(m_Origin.x + m_VisibleSize.width);
	return node;
}

void TutorialScene::initSceneObjects()
{
	//Camrea position is limited inside a specific Rect, its position is relative to camera
	Vec2 centerOfRect{ m_Origin.x + m_VisibleSize.width / 2, m_Origin.y + m_VisibleSize.height / 3 };
	Size sizeOfRect{ m_VisibleSize.width / 5, m_VisibleSize.width / 5 };
	m_CameraLimit = { centerOfRect.x - sizeOfRect.width / 2, centerOfRect.y - sizeOfRect.height / 2, sizeOfRect.width, sizeOfRect.height };

	//Alive limit, its x limit is relative to world, its y limit is relative to camera
	m_AliveLimit = { m_Origin.x - m_VisibleSize.width / 2, m_Origin.y, m_VisibleSize.width * 2, m_VisibleSize.height };

	// Tutorial
	m_DialBox = Sprite::create("img/dial.png");
	m_Message1 = Label::createWithTTF("welcome you to the game", RESOURCE(R_R_f_ethno_ttf), 24);
	auto imageSize = m_DialBox->getContentSize();
	m_Message1->setPosition(imageSize.width / 25, imageSize.height - imageSize.height / 7);
	m_Message1->setColor(Color3B::YELLOW);
	m_Message1->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	m_Message1->setOpacity(0);
	// add the label as a child to this layer
	m_DialBox->addChild(m_Message1);
	m_DialBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	Vec2 dgPos = Vec2(m_Origin.x + m_VisibleSize.width / 2, m_Origin.y + m_VisibleSize.height / 8);
	m_DialBox->setPosition(dgPos);
	addChild(m_DialBox, 10);
	this->schedule(SEL_SCHEDULE(&TutorialScene::type), 0.f);
	//--- End Tutorial
}

void TutorialScene::initEventListener()
{
	EventListenerTouchOneByOne *touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(TutorialScene::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
	EventListenerCustom *gameTutorial = EventListenerCustom::create("Tutorial", [this](EventCustom *event) {
		Director::getInstance()->pushScene(TransitionFade::create(1, TutorialScene::createScene()));
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(gameTutorial, this);
}

void TutorialScene::update(float duration)
{
	m_Background->m_Camera = m_Camera;
	if (!m_StartGame) //waiting for first tap
	{
		runEasyStage(duration);
	}

	if (m_StartGame) //GamePlay
	{
		runMeteoriteStage(duration);
	}
	updateAliveLimit();
	
	if (firstJump == true) {
		// Stop for new player tap
		auto characterPosition = m_Character->getPosition();
		Vec2 curPlanetPosition;
		if (m_CurrentPlanet != nullptr)
			curPlanetPosition = m_CurrentPlanet->getPosition();
		else
			curPlanetPosition = m_Planets.at(0)->getPosition();
		Vec2 nextPlanetPosition = m_Planets.at(1)->getPosition();
		Vec2 ab = characterPosition - curPlanetPosition;
		Vec2 ac = nextPlanetPosition - curPlanetPosition;
		if (Vec2::angle(ab, ac) <= 3.14 / 20) {
			count += 1;
			if (count == 10) {
				count = 0;
				m_CharacterAgSpeed = 0;
				firstJump = false;
				secondJump = true;
				m_Click = Sprite::create("img/Click0.png");
				Vector<SpriteFrame*> animFrames;
				animFrames.reserve(2);
				animFrames.pushBack(SpriteFrame::create("img/Click0.png",Rect(0,0,100,128)));
				animFrames.pushBack(SpriteFrame::create("img/Click1.png", Rect(0, 0, 92, 128)));
				Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
				Animate* animate = Animate::create(animation);
				m_Click->runAction(RepeatForever::create(animate));
				m_Click->setPosition(m_VisibleSize.width / 1.5, m_VisibleSize.height / 4);
				this->addChild(m_Click);
				isTutorial = false;
				//m_Message1->cleanup()
				m_Message1->setString("click to the screen to jump");
				m_Message1->setOpacity(0);
				m_Message1->setTTFConfig(_ttfConfig(RESOURCE(R_R_f_ethno_ttf), 25));
				this->schedule(SEL_SCHEDULE(&TutorialScene::type), 0.f);
			}
		}	
		
	}
	else if (secondJump == true && m_CurrentPlanet == m_Planets.at(1)) {
		
		m_Click->setVisible(true);
		isTutorial = true;
		auto characterPosition = m_Character->getPosition();
		Vec2 curPlanetPosition = m_CurrentPlanet->getPosition();
		Meteorite * meteorite = m_Meteorites.back();
		auto meteoritePosition = meteorite->getPosition();
		meteorite->work();
		if (characterPosition.x > curPlanetPosition.x) {
			m_CharacterAgSpeed = 0;
			thirdJump = true;
			m_Message1->setString("tap to jump to stay away\nfrom meteorites");
			m_Message1->setOpacity(0);
			m_Message1->setTTFConfig(_ttfConfig(RESOURCE(R_R_f_ethno_ttf), 25));
			this->schedule(SEL_SCHEDULE(&TutorialScene::type), 0.f);
			if (meteoritePosition.distance(curPlanetPosition) <= m_CurrentPlanet->getRadius() * 2.0f) {
				secondJump = false;
				thirdJump = true;
				isTutorial = false;
				veloTemp = meteorite->getVelocity();
				meteorite->stopMoveAndRotate();
			}
				
		}
	}
	else if (thirdJump == true && m_CurrentPlanet == m_Planets.at(1)) {
		m_Click->setVisible(true);
		//m_Meteorites.back()->stop();
		auto characterPosition = m_Character->getPosition();
		Vec2 curPlanetPosition = m_CurrentPlanet->getPosition();
		Vec2 nextPlanetPosition = m_Planets.at(2)->getPosition();
		Vec2 ab = characterPosition - curPlanetPosition;
		Vec2 ac = nextPlanetPosition - curPlanetPosition;
		CCLOG("%f", Vec2::angle(ab, ac));
		if (Vec2::angle(ab, ac) <= 3.14 / 20) {
			thirdJump = false;
			finishTutorial = true;
			m_Message1->setString("tap to jump to other\nplanets");
			m_Message1->setOpacity(0);
			m_Message1->setTTFConfig(_ttfConfig(RESOURCE(R_R_f_ethno_ttf), 25));
			this->schedule(SEL_SCHEDULE(&TutorialScene::type), 0.f);
			m_CharacterAgSpeed = 0;
		}
	}
	else if (finishTutorial == true && m_CurrentPlanet == m_Planets.at(2)) {
		m_Click->setVisible(true);
		isTutorial = true;
		auto characterPosition = m_Character->getPosition();
		Vec2 curPlanetPosition = m_CurrentPlanet->getPosition();
		m_Meteorites.back()->stop();
		if (abs (characterPosition.y - curPlanetPosition.y) < 10 && characterPosition.x < curPlanetPosition.x - m_CurrentPlanet->getRadius()) {
			m_CharacterAgSpeed = 0;
			m_Message1->setString("do not jump to the grand\nlines");
			m_Message1->setOpacity(0);
			m_Message1->setTTFConfig(_ttfConfig(RESOURCE(R_R_f_ethno_ttf), 25));
			this->schedule(SEL_SCHEDULE(&TutorialScene::type), 0.f);
			finishTutorial = false;
			isTutorial = false;
		}
		
	}
	updateAll(duration);
}

void TutorialScene::gameOver()
{
	m_GameOver = true;
	m_Character->stop();
	m_Camera->stopMove();
	auto particleX = ParticleSystemQuad::create(RESOURCE(R_R_particle_die_plist));
	particleX->setPosition(m_Character->getPosition());
	particleX->setVisible(true);
	particleX->setCameraMask((unsigned short)CameraFlag::USER1);
	world->addChild(particleX, 10);
	cocos2d::Device::vibrate(0.4f);
	//alert
	m_label = Label::createWithTTF("end tutorial", RESOURCE(R_R_f_spac_ttf), 40);
	m_label->setPosition(m_Origin.x + m_VisibleSize.width / 2, m_Origin.y + m_VisibleSize.height / 1.75);
	m_label->setOpacity(0);
	auto callback = CallFunc::create(this, callfunc_selector(TutorialScene::actionFinish));
	m_label->runAction(Sequence::create(FadeIn::create(2), callback, nullptr));
	this->addChild(m_label, 10);
}

bool TutorialScene::isMainCharOutOfCamera()
{
	return !m_AliveLimit.containsPoint(m_Character->getPosition()) && !m_CurrentPlanet;
}

bool TutorialScene::findCollisionPlanet()
{
	//find collision planet and save it
	//increase score if passing planets
	for (CPlanet *planet : m_Planets) {
		if (planet->getBoundingBox().containsPoint(m_Character->getPosition()) && planet->isVisible()) {
			Vec2 dir = m_Character->getPosition() - planet->getPosition();
			float dirLength = dir.length();
			float charRadius = m_Character->getRadius();
			float distance = planet->getRadius() + charRadius + charRadius / 4;
			if (dirLength <= distance) {
				auto particleX = ParticleSystemQuad::create(RESOURCE(R_R_particle_collision_plist));
				particleX->setPosition(planet->getPosition());
				particleX->setVisible(true);
				particleX->setCameraMask((unsigned short)CameraFlag::USER1);
				world->addChild(particleX, 10);
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
				m_Camera->stopMove();
				m_Character->setPosition(dir + planet->getPosition());
				m_CurrentPlanet = planet;
				return true;
			}
		}
	}
	return false;
}

bool TutorialScene::isCollideWithMeteorites()
{
	//check collision
	for (Meteorite *met : m_Meteorites) {
		if (met->isVisible()) {
			for (CPlanet *planet : m_Planets) {
				if (met->getBoundingBox().intersectsRect(planet->getBoundingBox())) {
					Vec2 dir = planet->getPosition() - met->getPosition();
					float dirLength = dir.length();
					float metRadius = met->getContentSize().height / 1.5;
					if (dirLength <= metRadius) {
						auto particleX = ParticleSystemQuad::create(RESOURCE(R_R_particle_star_plist));
						particleX->setVisible(true);
						particleX->setPosition(met->getPosition());
						particleX->setCameraMask((unsigned short)CameraFlag::USER1);
						world->addChild(particleX, 10);
						cocos2d::Device::vibrate(0.05f);
						particleX->runAction(Sequence::create(DelayTime::create(1.0), RemoveSelf::create(), NULL));
						met->setVisible(false);
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
					met->setVisible(false);
					gameOver();
					return true;
				}
			}
		}
	}
	return false;
}


void TutorialScene::rotateCharAroundPlanet(float duration)
{
	Vec2 dir = m_Character->getPosition() - m_CurrentPlanet->getPosition();
	//rotate character
	m_CharacterRotationTime += duration;
	dir = mtl::rotateZ(m_CharacterAgSpeed * duration, dir);
	m_Character->setPosition(dir + m_CurrentPlanet->getPosition());
	//move camera when out of view
	Vec2 planetPosInCamera = m_Camera->worldToCamera(m_CurrentPlanet->getPosition());
	Vec2 cameraPos = m_Camera->getPosition();
	if (!m_CameraLimit.containsPoint(planetPosInCamera)) {
		Vec2 direction = planetPosInCamera - Vec2(m_CameraLimit.getMidX(), m_CameraLimit.getMidY());
		m_Camera->setVelocity(direction.getNormalized() * m_WinSize.width);
	}
	else {
		m_Camera->stopMove();
	}
}

void TutorialScene::calculateMeteoritesVelocity(Meteorite * met)
{
	Vec2 moveDir = m_CurrentPlanet->getPosition() - met->getPosition();
	float totalDistance = moveDir.length();
	float oneRoundTime = std::abs(360 * m_NumRoundsForComing / m_CharacterAgSpeed);
	m_DistanceWhenComing = m_CurrentPlanet->getRadius() * 3;
	moveDir.normalize();
	met->setVelocity(moveDir * (totalDistance - m_DistanceWhenComing) / oneRoundTime);
}

//Generating meteorites: reuse meteorites, only change their positions
void TutorialScene::generateMeteorites()
{
	auto m_VisibleSize = Director::getInstance()->getVisibleSize();
	if (m_CurrentPlanet == m_Planets.at(1)) {
		Meteorite* met = m_Meteorites.front();
		m_Meteorites.pop_front();
		m_Meteorites.push_back(met);
		Vec2 position = Vec2::ZERO;
		position.x = int(-m_VisibleSize.width * 1.5);
		position.y = int(m_VisibleSize.height * 1.5);
		Vec2 worldPosition = m_Camera->cameraToWorld(position);
		met->setPosition(position);
		met->setVisible(true);

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

void TutorialScene::generateMeteoriteWarning() {
	//warning when having a meteorite
	if (thirdJump == true && m_CurrentPlanet == m_Planets.at(2))
		return;
	float meteDistanceFromPlanet = m_CurrentPlanet->getRadius() * 3;
	Vec2 planetToMeteoriteDir = m_Meteorites.back()->getPosition() - m_CurrentPlanet->getPosition();
	planetToMeteoriteDir.normalize();
	m_MeteWarning->setPosition(m_CurrentPlanet->getPosition() + planetToMeteoriteDir * meteDistanceFromPlanet);
	m_MeteWarning->work();
	m_MeteWarning->setOpacity(0);
	m_MeteWarning->stopAllActions();
	m_MeteWarning->runAction(Sequence::create(FadeIn::create(0.5),
		FadeOut::create(0.5), FadeIn::create(0.5), FadeOut::create(0.5),
		FadeIn::create(0.5), FadeOut::create(0.5), NULL));
}


//Update Alive limit when camera move
void TutorialScene::updateAliveLimit()
{
	m_AliveLimit.origin.y = m_Camera->getPositionY();
}


bool TutorialScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event * event)
{
	if (m_GameOver) return false;

	if (!m_StartGame) { m_StartGame = true; }

	if (!m_CurrentPlanet) {
		return false;
	}
	if (isTutorial == true)
		return false;
	m_Click->setVisible(false);
	if (secondJump == false && m_CurrentPlanet == m_Planets.at(1)) {
		m_Meteorites.back()->setVelocity(veloTemp);
	}
	SimpleAudioEngine::getInstance()->playEffect(RESOURCE(R_R_S_Jump_mp3));
	Vec2 dir = m_Character->getPosition() - m_CurrentPlanet->getPosition();
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
	m_CharacterRotationTime = 0;
	m_MeteWarning->stop();
	return true;
}

void TutorialScene::type(float dt)
{
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
		//isTutorial = false;
		unschedule(schedule_selector(TutorialScene::type));		
	}
}

void TutorialScene::menuSceneCallback(Ref* pSender)
{
	Director::getInstance()->pushScene(MainMenuScene::create());
}

void TutorialScene::actionFinish()
{
	Director::getInstance()->replaceScene(TransitionFade::create(1.0, GameScene::createScene()));
}

void TutorialScene::runEasyStage(float duration)
{
	if (m_GameOver) {
		return;
	}
	if (m_CurrentPlanet) {
		rotateCharAroundPlanet(duration);
	}
	else {
		if (findCollisionPlanet()) {
			SimpleAudioEngine::getInstance()->playEffect(RESOURCE(R_R_S_sfx_point_mp3));
		}
	}
	if (isMainCharOutOfCamera()) {
		gameOver();
		Director::getInstance()->replaceScene(TransitionFade::create(0.5, MainMenuScene::create()));
		return;
	}
}

void TutorialScene::runMeteoriteStage(float duration)
{
	if (m_GameOver) {
		return;
	}
	if (m_CurrentPlanet) {
		rotateCharAroundPlanet(duration);
	}
	else {
		if (findCollisionPlanet()) {
			//generating meteorites
			if (secondJump == true) {
				generateMeteorites();
				//generating warning
				generateMeteoriteWarning();

			}
			// Sound
			SimpleAudioEngine::getInstance()->playEffect(RESOURCE(R_R_S_sfx_point_mp3));
		}
	}
	isCollideWithMeteorites();
	if ((isMainCharOutOfCamera() || (m_CurrentPlanet && !m_CurrentPlanet->isVisible()))) {
		m_Click->setVisible(false);
		gameOver();		
		return;
	}
}

void TutorialScene::updateAll(float duration)
{
	/*m_Camera->update(duration);
	m_Background->update(duration);
	for (GObject * object : m_GObjects) {
		object->update(duration);
	}*/
	Vector<Node *> children = world->getChildren();
	for (Node * child : children) {
		child->update(duration);
	}
}