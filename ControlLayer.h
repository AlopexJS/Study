#pragma once
#ifndef _CONTROLLAYER_H_
#define _CONTROLLAYER_H_

#include "cocos2d.h"
USING_NS_CC;

class ControlDelegate
{
public:
	//distance��������ֱ�߾���
	virtual void onWalk(Vec2 direction, float distance) = 0;
	//����
	virtual void onAttack() = 0;
	virtual void onHeavyAttack() = 0;
	//ֹͣ�ƶ�
	virtual void onStop() = 0;
	//�����ܻ���Ӣ���ܻ���Ҫ��gamelayer����д
	virtual void onHurt() = 0;
};


class ControlLayer :public Layer
{
public:

	ControlLayer();
	~ControlLayer();

	virtual bool init();
	bool attackContain(Vec2 pos);
	bool heavyAttackContain(Vec2 pos);
	CREATE_FUNC(ControlLayer);
	

	//���ص������¼���������
	void onTouchesBegan(const std::vector<Touch*>& touches, Event* e);
	void onTouchesMoved(const std::vector<Touch*>& touches, Event* e);
	void onTouchesEnded(const std::vector<Touch*>& touches, Event* e);
	CC_SYNTHESIZE(ControlDelegate*, _delegator, Delegator);//�趨�ӿ�

private:

	//�������ͻ���
	Sprite* _joypad;
	Sprite* _joypad_bg;

	//����ͼ��
	Sprite* _attack;
	Sprite* _heavyAttack;

	//�������ܲ�ִ����ȴ����
	void heavyAttackAction(Node* target, int time);

	//�����������������������
	void _activityJoypad(Vec2 position);
	//���ؿ�����
	void _inactivityJoypad();
	//ˢ�¿����������λ�ã����ݴ������ƣ�
	void _updateJoypad(Vec2 direction, float distance);
};


#endif // !_CONTROLLAYER_H_