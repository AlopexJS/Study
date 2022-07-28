#pragma once
#ifndef _CONTROLLAYER_H_
#define _CONTROLLAYER_H_

#include "cocos2d.h"
USING_NS_CC;

class ControlDelegate
{
public:
	//distance是与起点的直线距离
	virtual void onWalk(Vec2 direction, float distance) = 0;
	//攻击
	virtual void onAttack() = 0;
	virtual void onHeavyAttack() = 0;
	//停止移动
	virtual void onStop() = 0;
	//怪物受击，英雄受击需要再gamelayer里面写
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
	

	//触控的三个事件函数重载
	void onTouchesBegan(const std::vector<Touch*>& touches, Event* e);
	void onTouchesMoved(const std::vector<Touch*>& touches, Event* e);
	void onTouchesEnded(const std::vector<Touch*>& touches, Event* e);
	CC_SYNTHESIZE(ControlDelegate*, _delegator, Delegator);//设定接口

private:

	//控制器和基座
	Sprite* _joypad;
	Sprite* _joypad_bg;

	//技能图标
	Sprite* _attack;
	Sprite* _heavyAttack;

	//发动技能并执行冷却动画
	void heavyAttackAction(Node* target, int time);

	//激活控制器，并更新其坐标
	void _activityJoypad(Vec2 position);
	//隐藏控制器
	void _inactivityJoypad();
	//刷新控制器的相对位置（根据触控手势）
	void _updateJoypad(Vec2 direction, float distance);
};


#endif // !_CONTROLLAYER_H_