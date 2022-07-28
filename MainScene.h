

#ifndef __MAINSCENE_H__
#define __MAINSCENE_H__

#include "cocos2d.h"
#include "GameLayer.h"

USING_NS_CC;

class MainScene : public cocos2d::Scene
{
protected:

	GameLayer* _gameLayer;

public:
	virtual ~MainScene();
	static cocos2d::Scene* createScene();

    virtual bool init();
    
	void menuCloseCallback(Ref* pSender);
    CREATE_FUNC(MainScene);
};

#endif 