#pragma once
#ifndef _SPRITEACTION_H_
#define _SPRITEACTION_H_

#include "cocos2d.h"

using namespace cocos2d;

class SpriteAction : public Sprite
{
protected:
	SpriteAction();

	FiniteTimeAction *m_pActionIdle;
	FiniteTimeAction *m_pActionWalk;
	FiniteTimeAction *m_pActionAttack;
	FiniteTimeAction *m_pActionHurt;
	FiniteTimeAction *m_pActionDeath;

	LabelTTF *m_pLossBlood;

public:

	CC_SYNTHESIZE(bool, isWalking, IsWalking);//只要是重复的动作都需要判断是否在进行
	CC_SYNTHESIZE(bool, isIdling, IsIdling);
	CC_SYNTHESIZE(bool, isAttack, IsAttack);
	CC_SYNTHESIZE(bool, isDeath, IsDeath);
	CC_SYNTHESIZE(float, m_fBloodMax, BloodMax);
	CC_SYNTHESIZE(float, m_fBloodNow, BloodNow);
	CC_SYNTHESIZE(Vec2, _direction, Direction);//用于判断转向


	virtual ~SpriteAction();
	virtual bool init();

	virtual void idle();
	virtual void walk(Vec2 direction);//由于gamemonster的实现不一样，所以只由gamehero继承
	virtual void attack();
	virtual void hurt(int bd);
	virtual void bloodDown(int bd);
	virtual void death();
	virtual void cleanUp();
	virtual void onExit();


	void setRestrPosition(Vec2 pos);
	//virtual void update(float dt);//运动时更新角色状态,现在暂时写在gamelayer里面

};

#endif // !_SPRITEACTION_H_