#pragma once

#ifndef _GAMELAYER_H_
#define _GAMELAYER_H_

#include "cocos2d.h"
#include "GameRole.h"
#include "ControlLayer.h"
#include <vector>

class GameLayer : public cocos2d::Layer, public ControlDelegate
{
protected:
	//创建的成员记住在退出时进行清理

	GameHero* hero;Vec2 posHero;
	//TMXTiledMap* _tileMap;

	std::vector<SpriteAction*> roleVector;//用于遍历时随时储存英雄的位置
	ControlLayer* _controlLayer;

	Sprite* bloodBar1;
	Sprite* bloodBar2;

	ProgressTimer* bloodProgress;
	//Sprite* headProfile;
public:
	virtual ~GameLayer();

	virtual bool init();
	CREATE_FUNC(GameLayer);

	virtual void onWalk(Vec2 direction, float distance);
	virtual void onAttack();
	virtual void onHeavyAttack();
	virtual void onStop();
	virtual void onHurt();


	virtual void update(float dt);
	void updateAi(float dt);
	void monsterGetHurt();
	void heroGetHurt();
};

#endif