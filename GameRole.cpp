#include "GameRole.h"

GameHero::GameHero()
{
	
}

bool GameHero::init()
{
	bool bRet = 0;
	do
	{
		CC_BREAK_IF(!SpriteAction::init());
		this->initWithFile("demo/stand_1.png");
		
		_direction = Vec2::ZERO;
		_velocity = Vec2::ZERO;
		_flippedX = false;
		isWalking = false;
		isIdling = false;
		isAttack = false;

		m_fBloodNow = 600;
		m_fBloodMax = 600;

		m_eType = GAME_HERO;
		m_eName = WARRIOR_T1;
		baseAtk = 20;
		atk = baseAtk;
		m_fAttackRange = 0;
		m_iID = 1;

		initAction();

		m_pActionLevelUp = NULL;//先设置为NULL
		m_pActionHurt = NULL;
		m_pActionDeath = NULL;

		bRet = true;
	} while (0);
	return true;
}

void GameHero::initAction()//帧动画，还需配合基本动画
{
	// Idle动作
	Animation *pAnimIdle = Animation::create();
	char szFile[50] = { 0 };
	float standUnitDelay = 5.0f / 60;
	for (int i = 1; i <= 8; ++i)
	{
		sprintf(szFile, "demo/stand_%d.png", i);
		pAnimIdle->addSpriteFrameWithFileName(szFile);//虽然名字时spriteFrame，但还是要从纹理缓存中提取
	}
	pAnimIdle->setDelayPerUnit(standUnitDelay);
	m_pActionIdle = (FiniteTimeAction *)RepeatForever::create(Animate::create(pAnimIdle));
	m_pActionIdle->setDuration(8 * standUnitDelay);
	m_pActionIdle->setTag(Action_Type::ACTION_TYPE_IDLE);
	m_pActionIdle->retain();
	
	//Walk动作
	Animation *pAnimWalk = Animation::create();
	float walkUnitDelay = 4.0 / 60;
	for (int i = 1; i <= 4; ++i)
	{
		sprintf(szFile, "demo/walk_%d.png", i);
		pAnimWalk->addSpriteFrameWithFileName(szFile);
	}
	pAnimWalk->setDelayPerUnit(walkUnitDelay);
	m_pActionWalk = (FiniteTimeAction *)RepeatForever::create(Animate::create(pAnimWalk));
	m_pActionWalk->setDuration(4 * walkUnitDelay);
	m_pActionWalk->setTag(Action_Type::ACTION_TYPE_WALK);
	m_pActionWalk->retain();

	// 普通攻击
	Animation *pAttackNormal = Animation::create();
	float normalUnitDelay = 2.5 / 60;
	for (int i = 1; i <= 3; ++i)
	{
		sprintf(szFile, "demo/stand_weak_attack_%d.png", i);
		pAttackNormal->addSpriteFrameWithFileName(szFile);
	}
	pAttackNormal->setDelayPerUnit(normalUnitDelay);
	m_pActionAttack = Sequence::create(Animate::create(pAttackNormal), callbackIdle, NULL);//最好用回调形式，停止后再运行时不连贯的
	m_pActionAttack->setDuration(3 * normalUnitDelay);
	m_pActionAttack->setTag(Action_Type::ACTION_TYPE_ATTACK);
	m_pActionAttack->retain();

	//重击
	Animation *pAttackHeavy = Animation::create();
	float heavyUnitDelay = 10.0 / 60;
	for (int i = 1; i <= 6; ++i)
	{
		sprintf(szFile, "demo/stand_hard_attack_%d.png", i);
		pAttackHeavy->addSpriteFrameWithFileName(szFile);
	}
	pAttackHeavy->setDelayPerUnit(heavyUnitDelay);
	m_pActionHeavyAttack = Sequence::create(Animate::create(pAttackHeavy), callbackIdle, NULL);//与瞬时动作，不改变位置与attack一样
	m_pActionHeavyAttack->setDuration(6 * heavyUnitDelay);
	m_pActionHeavyAttack->setTag(Action_Type::ACTION_TYPE_HEAVYATTACK);
	m_pActionHeavyAttack->retain();
}


GameHero::~GameHero()
{
	CC_SAFE_RELEASE(m_pActionLevelUp);
	CC_SAFE_RELEASE(m_pActionHeavyAttack);
	CC_SAFE_RELEASE(callbackIdle);
}

void GameHero::idle()
{
	if (!isIdling)
	{
		this->stopAllActions();
		this->runAction(m_pActionIdle);
		this->setIsIdling(true);
		this->setIsWalking(false);
		this->setIsAttack(false);
	}
}

void GameHero::walk(Vec2 direction)
{
	if (!isWalking)
	{
		this->stopAllActions();
		this->runAction(m_pActionWalk);
	}
	_direction = direction;
	if (_direction.x > 0 && _flippedX == true)
		_flippedX = false;
	if (_direction.x < 0 && _flippedX == false)
		_flippedX = true;
}

void GameHero::attack()
{
	this->stopAllActions();
	this->runAction(m_pActionAttack);
	this->setAttackRange(20);
	this->setAtk(baseAtk);
	
	this->setIsAttack(true);
	this->setIsWalking(false);
	this->setIsIdling(false);
}

void GameHero::heavyAttack()
{
	this->stopAllActions();
	this->runAction(m_pActionHeavyAttack);
	this->setAttackRange(50);
	this->setAtk(baseAtk+30);

	this->setIsAttack(true);
	this->setIsWalking(false);
	this->setIsIdling(false);
}

void GameHero::hurt(int bd)
{
}

void GameHero::death()
{

}

void GameHero::onExit()
{
	SpriteAction::onExit();
}

void GameHero::levelUp()
{
}





GameMonster::GameMonster()
{
}

GameMonster::~GameMonster()
{
	CC_SAFE_RELEASE(bloodSprite);
	CC_SAFE_RELEASE(bloodBackground);
	CC_SAFE_RELEASE(bloodBackgroundBar);
	CC_SAFE_RELEASE(callbackDeath);
}

bool GameMonster::init()
{
	bool bRet = 0;
	do
	{
		CC_BREAK_IF(!SpriteAction::init());
		//this->initWithFile("demo2/idle_1.png");//使用了精灵节点，所以会导致纹理不同的错误，可以用initwithspriteframe替代
		auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("walk_1.png");
		this->initWithSpriteFrame(spriteFrame);
		
		_direction = Vec2(-1,0);
		_flippedX = false;
		isWalking = false;
		isIdling = false;
		isAttack = false;

		m_eType = GAME_MONSTER;
		m_eName = WARRIOR_T2;
		baseAtk = 5;
		m_fAttackRange = 0;

		initAction();
		initBlood();

		bRet = true;
	} while (0);
	return true;
}

void GameMonster::initAction()
{
	
	// Idle动作
	Animation *pAnimIdle = Animation::create();
	char szFile[50] = { 0 };
	float standUnitDelay = 12.0f / 60;
	for (int i = 1; i <= 8; ++i)
	{
		sprintf(szFile, "idle_%d.png", i);
		auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFile);//从精灵帧缓存中加载
		pAnimIdle->addSpriteFrame(spriteFrame);
	}
	pAnimIdle->setDelayPerUnit(standUnitDelay);
	m_pActionIdle = (FiniteTimeAction *)RepeatForever::create(Animate::create(pAnimIdle));
	m_pActionIdle->setDuration(8 * standUnitDelay);
	m_pActionIdle->setTag(Action_Type::ACTION_TYPE_IDLE);
	m_pActionIdle->retain();

	//Walk动作
	Animation *pAnimWalk = Animation::create();
	float walkUnitDelay = 12.0 / 60;
	for (int i = 1; i <= 8; ++i)
	{
		sprintf(szFile, "walk_%d.png", i);
		auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFile);
		pAnimWalk->addSpriteFrame(spriteFrame);
	}
	pAnimWalk->setDelayPerUnit(walkUnitDelay);
	m_pActionWalk = (FiniteTimeAction *)RepeatForever::create(Animate::create(pAnimWalk));
	m_pActionWalk->setDuration(8 * walkUnitDelay);
	m_pActionWalk->setTag(Action_Type::ACTION_TYPE_WALK);
	m_pActionWalk->retain();

	// 普通攻击
	Animation *pAttackNormal = Animation::create();
	float normalUnitDelay = 12.0 / 60;
	for (int i = 1; i <= 8; ++i)
	{
		sprintf(szFile, "attack_%d.png", i);
		auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFile);
		pAttackNormal->addSpriteFrame(spriteFrame);
	}
	pAttackNormal->setDelayPerUnit(normalUnitDelay);
	m_pActionAttack = Animate::create(pAttackNormal);//由于怪物没有人操控，动作随机，不需要恢复idle状态
	m_pActionAttack->setDuration(8 * normalUnitDelay);
	m_pActionAttack->setTag(Action_Type::ACTION_TYPE_ATTACK);
	m_pActionAttack->retain();

	//受击
	Animation *pAnimHurt = Animation::create();
	float hurtUnitDelay = 12.0 / 60;
	for (int i = 1; i <= 6; ++i)
	{
		sprintf(szFile, "hurt_%d.png", i);
		auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFile);
		pAnimHurt->addSpriteFrame(spriteFrame);
	}
	pAnimHurt->setDelayPerUnit(hurtUnitDelay);
	m_pActionHurt = Animate::create(pAnimHurt);
	m_pActionHurt->setDuration(6 * hurtUnitDelay);
	m_pActionHurt->setTag(Action_Type::ACTION_TYPE_HURT);
	m_pActionHurt->retain();

	//死亡
	Animation *pAnimDeath = Animation::create();
	float deathUnitDelay = 12.0 / 60;
	for (int i = 1; i <= 3; ++i)
	{
		sprintf(szFile, "death_%d.png", i);
		auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szFile);
		pAnimDeath->addSpriteFrame(spriteFrame);
	}
	pAnimDeath->setDelayPerUnit(deathUnitDelay);
	m_pActionDeath = Sequence::create(Animate::create(pAnimDeath), callbackDeath, NULL);
	m_pActionDeath->setDuration(3 * deathUnitDelay);
	m_pActionDeath->setTag(Action_Type::ACTION_TYPE_DEATH);
	m_pActionDeath->retain();
}

void GameMonster::ai()
{
	auto rand = CCRANDOM_0_1() > 0.8 ? 1 : 0;
	switch (rand)
	{
	case(0):
		this->walk();
		break;
	case(1):
		this->attack();
		break;
	}
}

void GameMonster::idle()
{
	if (!isIdling)
	{
		this->stopAllActions();
		this->runAction(m_pActionIdle);
		this->setIsIdling(true);
		this->setIsAttack(false);
		this->setIsWalking(false);
	}
}

void GameMonster::walk()
{

	if (!isWalking)
	{
		this->stopAllActions();
		this->runAction(m_pActionWalk);
		this->setIsWalking(true);
		this->setIsIdling(false);
		this->setIsAttack(false);
	}
}

void GameMonster::attack()
{
	if (!isAttack)
	{
		this->stopAllActions();
		this->runAction(m_pActionAttack);
		this->setIsAttack(true);
		this->setIsWalking(false);
		this->setIsIdling(false);
	}
}

void GameMonster::hurt(int bd)
{
	m_fBloodNow -= bd;
	if (this->getBloodNow() > 0)
	{
		this->stopAllActions();
		this->runAction(m_pActionHurt);
		this->setIsAttack(false);
		this->setIsWalking(false);
		this->setIsIdling(false);
	}
	else
	{
		this->death();
	}
}

void GameMonster::death()
{
	this->stopAllActions();
	this->runAction(m_pActionDeath);
	this->setIsAttack(true);
	this->setIsWalking(true);
	this->setIsIdling(true);
}

void GameMonster::cleanUp()
{
	this->setVisible(false);
	m_pActionIdle->release();
	m_pActionWalk->release();
	m_pActionAttack->release();
	m_pActionHurt->release();
	m_pActionDeath->release();
	
	bloodBackgroundBar->stopAllActions();
	m_pLossBlood->stopAllActions();//之前没有注意到这两个还有action在运行，所以总是提醒is_Running错误
	
	/*m_pLossBlood->release();
	bloodBackgroundBar->release();*/
	/*bloodSprite->release();
	callbackDeath->release();*///由于没有retain，不需要自己释放

	this->setIsDeath(true);
}

void GameMonster::onExit()
{
	SpriteAction::onExit();
}


void GameMonster::changeDirection(Vec2 direction)
{
	//调整怪物方向
	_direction = direction;//这里的direction与英雄的一样，只起到调整面向的作用,在update中实现
	if (_direction.x > 0 && _flippedX == false)
		_flippedX = true;
	if (_direction.x < 0 && _flippedX == true)
		_flippedX = false;
}

void GameMonster::initBlood()
{
	m_fBloodNow = 120;
	m_fBloodMax = 120;

	bloodBackground = Sprite::create("demo2/bloodBackground.png");
	bloodSprite = Sprite::create("demo2/bloodSprite.png");
	bloodBackground->setContentSize(Size(this->getContentSize().width, 5));;
	
	bloodSprite->setContentSize(Size(this->getContentSize().width, 5));
	bloodSprite->setAnchorPoint(Vec2::ZERO);
	bloodSprite->setPosition(0, 100);
	this->addChild(bloodSprite);
	
	bloodBackgroundBar = ProgressTimer::create(bloodBackground);
	bloodBackgroundBar->setType(ProgressTimerType::BAR);
	bloodBackgroundBar->setAnchorPoint(Vec2::ZERO);
	bloodBackgroundBar->setPosition(0, 100);
	bloodBackgroundBar->setMidpoint(Vec2(1, 0));//从右到左
	bloodBackgroundBar->setBarChangeRate(Vec2(1, 0));//只在水平方向改变

	this->addChild(bloodBackgroundBar);
}

void GameMonster::bloodDown(int bd)
{
	if (m_pLossBlood)
	{
		this->removeChild(m_pLossBlood, true);
	}
	char temp[20] = { 0 };
	sprintf(temp, "-%d", bd);
	m_pLossBlood = LabelTTF::create(temp, "Tahoma", 20);
	m_pLossBlood->setPosition(Vec2(this->getContentSize().width / 2 + 10, this->getContentSize().height / 2));
	this->addChild(m_pLossBlood,800);

	FiniteTimeAction* fadeIn = FadeIn::create(0.8);
	FiniteTimeAction* fadeOut = FadeOut::create(1);
	FiniteTimeAction* move = MoveBy::create(1, Vec2(0, -10));
	Action *action = Spawn::create(fadeIn, fadeOut, move, NULL);
	
	
	if (m_fBloodNow + bd >= 0)//血量判断
	{
		m_pLossBlood->runAction(action);

		auto bloodAction = ProgressTo::create(0.5, 100 - (m_fBloodNow * 100 / m_fBloodMax));//运用progressTimer动画，覆盖红血条
		bloodBackgroundBar->runAction(bloodAction);
	}
}
