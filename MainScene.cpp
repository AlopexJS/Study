
#include "MainScene.h"
#include "SimpleAudioEngine.h"


MainScene::~MainScene()
{
	CC_SAFE_RELEASE(_gameLayer);
}

Scene* MainScene::createScene()
{
    return MainScene::create();
}

bool MainScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
	auto visibleSize = Director::getInstance()->getWinSize();

	auto backGround = Sprite::create("background.png");
	//backGround->ignoreAnchorPointForPosition(true);
	backGround->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(backGround);
    
	_gameLayer = GameLayer::create();
	_gameLayer->setPosition(0,0);
	this->addChild(_gameLayer);

	return true;
}

void MainScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
