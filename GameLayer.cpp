
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
	hero->setIsWalking(true);//设置运动状态很重要，这样切换运动状态才顺畅
	hero->setIsIdling(false);
	hero->setVelocity(direction * (distance < 128 ? 1.5 : 3));//速度为每一帧移动的距离，在update函数中实现
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
	//移动需要调用更新，限制hero位置
	if (hero->getIsWalking())
	{
		auto posNext = hero->getPosition() + hero->getVelocity();
		hero->setRestrPosition(posNext);
	}
	
	//调整精灵的Zorder、方向与位置
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
			
			roleMonster->changeDirection(dir);//与英雄不一样，怪物需要实时改变方向（根据自己与英雄的向量方向）,英雄的写在了自己的函数里面
			
			//限制怪物的位置
			if (role->getIsWalking())
			{
				auto posNext = roleMonster->getPosition()+roleMonster->getPosDecision();
				role->setRestrPosition(posNext);
			}
		}
	}

}

void GameLayer::updateAi(float dt)//简单动作与位置AI实现，每动画时长执行一次,这个很重要，别放进update函数中
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
						this->removeChild(roleMonster);//整了一天终于在这里搞好了，因为有太多的action在同时运行，清理时必须把全部action都停止掉，而且要理清动画之间的顺序，最好写在死亡动画之后，用回调函数形式进行清理，最后给一个死亡标记，在实际运行的layer中remove掉。同时有些动画是在init函数中创建的，需要retain，而有些动画是动态创建的，需要分清两种动画的关系，什么时候用什么动画
					}
				}
			}
		}
	}
}

void GameLayer::heroGetHurt()
{
}

