#pragma once
#ifndef _GAMEROLE_H_
#define _GAMEROLE_H_

#include "cocos2d.h"
#include "SpriteAction.h"
using namespace cocos2d;

typedef enum {
	ACTION_TYPE_IDLE,//ÿһ�ֶ���Ҳ�����ж��ֶ���״̬
	ACTION_TYPE_WALK,
	ACTION_TYPE_ATTACK,
	ACTION_TYPE_HEAVYATTACK,
	ACTION_TYPE_HURT,
	ACTION_TYPE_DEATH,
	ACTION_TYPE_LEVELUP
}Action_Type;


typedef enum {
	GAME_MONSTER,
	GAME_HERO,
	GAME_HUMAN
} Role_Type;

typedef enum {
	ARCHER,
	WARRIOR_T1,
	WARRIOR_T2,
}Role_Name;
class GameHero : public SpriteAction
{
protected:
	GameHero();

	FiniteTimeAction *m_pActionLevelUp;
	FiniteTimeAction *m_pActionHeavyAttack;

public:
	virtual ~GameHero();
	CREATE_FUNC(GameHero);

	virtual bool init();
	void initAction();
	CallFunc *callbackIdle = CallFunc::create(std::bind(&GameHero::idle, this));

	virtual void idle();
	virtual void walk(Vec2 direction);
	virtual void attack();
	virtual void hurt(int bd);
	virtual void death();
	virtual void onExit();

	CC_SYNTHESIZE(Role_Type, m_eType, RoleType);
	CC_SYNTHESIZE(Role_Name, m_eName, RoleName);
	CC_SYNTHESIZE(float, baseAtk, BaseAtk);//����ʱ�ı�
	CC_SYNTHESIZE(float, atk, Atk);//ʹ�ü���ʱ�ı�
	CC_SYNTHESIZE(float, m_fAttackRange, AttackRange);
	CC_SYNTHESIZE(Vec2, _velocity, Velocity);//���ڸ���λ�ã���ÿһ֡�ƶ��ľ���
	CC_SYNTHESIZE(int, m_iID, RoleID);

	void levelUp();
	void heavyAttack();
};

class GameMonster:public SpriteAction
{
protected:
	GameMonster();

public:
	virtual ~GameMonster();

	CREATE_FUNC(GameMonster);
	virtual bool init();
	void initAction();
	CallFunc *callbackDeath = CallFunc::create(std::bind(&GameMonster::cleanUp, this));

	CC_SYNTHESIZE(Role_Type, m_eType, RoleType);
	CC_SYNTHESIZE(Role_Name, m_eName, RoleName);
	CC_SYNTHESIZE(Vec2, _posDecision, PosDecision);//AI��������
	CC_SYNTHESIZE(float, baseAtk, BaseAtk);
	CC_SYNTHESIZE(int, m_iID, RoleID);
	CC_SYNTHESIZE(float, m_fAttackRange, AttackRange);

	Sprite* bloodSprite;
	Sprite* bloodBackground;
	ProgressTimer* bloodBackgroundBar;

	virtual void ai();

	virtual void idle();
	virtual void walk();//��hero��ʵ�ַ�ʽ��һ��
	virtual void attack();
	virtual void hurt(int bd);
	virtual void death();
	virtual void cleanUp();
	virtual void onExit();

	void changeDirection(Vec2 direction);//AI���к���
	void initBlood();
	virtual void bloodDown(int bd);
};

#endif // !_GAMEROLE_H_
