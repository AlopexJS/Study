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

	CC_SYNTHESIZE(bool, isWalking, IsWalking);//ֻҪ���ظ��Ķ�������Ҫ�ж��Ƿ��ڽ���
	CC_SYNTHESIZE(bool, isIdling, IsIdling);
	CC_SYNTHESIZE(bool, isAttack, IsAttack);
	CC_SYNTHESIZE(bool, isDeath, IsDeath);
	CC_SYNTHESIZE(float, m_fBloodMax, BloodMax);
	CC_SYNTHESIZE(float, m_fBloodNow, BloodNow);
	CC_SYNTHESIZE(Vec2, _direction, Direction);//�����ж�ת��


	virtual ~SpriteAction();
	virtual bool init();

	virtual void idle();
	virtual void walk(Vec2 direction);//����gamemonster��ʵ�ֲ�һ��������ֻ��gamehero�̳�
	virtual void attack();
	virtual void hurt(int bd);
	virtual void bloodDown(int bd);
	virtual void death();
	virtual void cleanUp();
	virtual void onExit();


	void setRestrPosition(Vec2 pos);
	//virtual void update(float dt);//�˶�ʱ���½�ɫ״̬,������ʱд��gamelayer����

};

#endif // !_SPRITEACTION_H_