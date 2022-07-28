
#include "GameLayer.h"
#define MAPTILESIZE Size(96,54)

GameLayer::~GameLayer()
{
	CC_SAFE_RELEASE(hero);
	CC_SAFE_RELEASE(_controlLayer);
	CC_SAFE_RELEASE(bloodBar1);
	CC_SAFE_RELEASE(bloodBar2);
	CC_SAFE_RELEASE(bloodProgress);
}

bool GameLayer::init()
{
	bool bRet = false;
	do
	{
		if (!Layer::init())
		{
			return false;
		}
		/*_tileMap = TMXTiledMap::create("background.tmx");
		_tileMap->setPosition(0,0);
		this->addChild(_tileMap);*/

		hero = GameHero::create();
		hero->ignoreAnchorPointForPosition(true);
		hero->setPosition(MAPTILESIZE.width * 2, MAPTILESIZE.height * 3);
		hero->setTag(1);
		this->addChild(hero);
		hero->idle();
		roleVector.push_back(hero);


		bloodBar1 = Sprite::create("demo/bloodBar1.png");
		bloodBar1->ignoreAnchorPointForPosition(true);
		bloodBar1->setPosition(30, 450);
		this->addChild(bloodBar1, 1000);

		bloodBar2 = Sprite::create("demo/bloodBar2.png");
		bloodProgress = ProgressTimer::create(bloodBar2);
		bloodProgress->ignoreAnchorPointForPosition(true);
		bloodProgress->setType(ProgressTimerType::BAR);
		bloodProgress->setMidpoint(Vec2(1, 0));
		bloodProgress->setBarChangeRate(Vec2(1, 0));
		bloodProgress->setPosition(30, 450);
		this->addChild(bloodProgress, 1000);

		/*headProfile = Sprite::create("demo/headProfile.png");
		headProfile->ignoreAnchorPointForPosition(true);
		headProfile->setPosition(0, 350);
		this->addChild(headProfile, 1000);*/


		_controlLayer = ControlLayer::create();
		_controlLayer->setPosition(Vec2::ZERO);
		_controlLayer->setDelegator(this);
		this->addChild(_controlLayer,1000);

		
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("demo2/foxsprite.plist");
		
		//auto spriteNode = SpriteBatchNode::create("demo2/foxsprite.png");
		//for (size_t i = 1; i <= 2; i++)
		//{
		//	auto gameMonster = GameMonster::create();
		//	gameMonster->setPosition(CCRANDOM_0_1() * 960, (CCRANDOM_0_1()*0.6 + 0.4) * 270);
		//	gameMonster->idle();
		//	spriteNode->addChild(gameMonster);
		//}
		//spriteNode->setPosition(Vec2::ZERO);
		//this->addChild(spriteNode);

		for (int i = 1; i <= 3; i++)
		{
			auto gameMonster = GameMonster::create();
			gameMonster->ignoreAnchorPointForPosition(true);
			gameMonster->setPosition(CCRANDOM_0_1() * 960, (CCRANDOM_0_1()*0.6 + 0.4) * 270);
			gameMonster->walk();
			gameMonster->setTag(i + 1);
			this->addChild(gameMonster);
			roleVector.push_back(gameMonster);
		}

		scheduleUpdate();
		schedule(schedule_selector(GameLayer::updateAi), 1.6, CC_REPEAT_FOREVER, 0.0f);
		bRet = true;
	} while (0);
	return bRet;
}

void GameLayer::onWalk(Vec2 direction, float distance)
{
	hero->walk(direction);
	hero->setIsWalking(true);//�����˶�״̬����Ҫ�������л��˶�״̬��˳��
	hero->setIsIdling(false);
	hero->setVelocity(direction * (distance < 128 ? 1.5 : 3));//�ٶ�Ϊÿһ֡�ƶ��ľ��룬��update������ʵ��
}

void GameLayer::onAttack()
{
	hero->attack();
}

void GameLayer::onHeavyAttack()
{
	hero->heavyAttack();
}

void GameLayer::onStop()
{
	hero->idle();
}

void GameLayer::onHurt()
{
	monsterGetHurt();
}

void GameLayer::update(float dt)
{
	//�ƶ���Ҫ���ø��£�����heroλ��
	if (hero->getIsWalking())
	{
		auto posNext = hero->getPosition() + hero->getVelocity();
		hero->setRestrPosition(posNext);
	}
	
	//���������Zorder��������λ��
	for (auto role : roleVector)
	{
		auto posy = 10000/role->getPosition().y;
		role->setZOrder(floor(posy));
		
		if (role->getTag() == 1)
		{
			auto posHeroNow = role->getPosition();
			posHero = posHeroNow;
		}
		if (role->getTag() != 1)
		{
			auto dir = posHero - role->getPosition();
			auto roleMonster = (GameMonster*)role;
			
			roleMonster->changeDirection(dir);//��Ӣ�۲�һ����������Ҫʵʱ�ı䷽�򣨸����Լ���Ӣ�۵���������,Ӣ�۵�д�����Լ��ĺ�������
			
			//���ƹ����λ��
			if (role->getIsWalking())
			{
				auto posNext = roleMonster->getPosition()+roleMonster->getPosDecision();
				role->setRestrPosition(posNext);
			}
		}
	}

}

void GameLayer::updateAi(float dt)//�򵥶�����λ��AIʵ�֣�ÿ����ʱ��ִ��һ��,�������Ҫ����Ž�update������
{
	for (auto role : roleVector)
	{
		if (role->getTag() != 1)
		{
			auto posDecision = Vec2::ZERO;
			auto dir = posHero - role->getPosition();
			if (dir.getLength() > 200.0f)
			{
				posDecision = (CCRANDOM_0_1() > 0.3 ? 1 : -1)*dir.getNormalized();
			}
			else
			{
				posDecision = (CCRANDOM_0_1() > 0.7 ? 1 : -1)*dir.getNormalized();
			}
			auto roleMonster = (GameMonster*)role;
			roleMonster->setPosDecision(posDecision);
			roleMonster->ai();
		}
	}

	hero->setBloodNow(hero->getBloodNow() - 50);
	auto bloodAction = ProgressTo::create(0.5, 100 - hero->getBloodNow() * 100 / hero->getBloodMax());
	bloodProgress->runAction(bloodAction);
}

void GameLayer::monsterGetHurt()
{
	for (auto role = roleVector.begin();role != roleVector.end();role++)
	{
		if ((*role)->getTag() != 1)
		{
			if (hero->getIsAttack())
			{
				auto sizeH = Size(hero->getContentSize().width + hero->getAttackRange(), hero->getContentSize().height);
				auto rectH = Rect(hero->getPosition(), sizeH);
				auto sizeM = (*role)->getContentSize();
				auto rectM = Rect((*role)->getPosition(), sizeM);
				if (rectH.intersectsRect(rectM) && hero->getPosition().y <= (*role)->getPosition().y + 10 && hero->getPosition().y >= (*role)->getPosition().y - 10)
				{
					auto roleMonster = (GameMonster*)(*role);
					roleMonster->hurt(hero->getAtk());
					roleMonster->bloodDown(hero->getAtk());
					if (roleMonster->getIsDeath())
					{
						this->removeChild(roleMonster);//����һ���������������ˣ���Ϊ��̫���action��ͬʱ���У�����ʱ�����ȫ��action��ֹͣ��������Ҫ���嶯��֮���˳�����д����������֮���ûص�������ʽ������������һ��������ǣ���ʵ�����е�layer��remove����ͬʱ��Щ��������init�����д����ģ���Ҫretain������Щ�����Ƕ�̬�����ģ���Ҫ�������ֶ����Ĺ�ϵ��ʲôʱ����ʲô����
					}
				}
			}
		}
	}
}

void GameLayer::heroGetHurt()
{
}

