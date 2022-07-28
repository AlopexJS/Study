#include "ControlLayer.h"

ControlLayer::ControlLayer()
{
}

ControlLayer::~ControlLayer()
{
	CC_SAFE_RELEASE(_joypad);
	CC_SAFE_RELEASE(_joypad_bg);
	CC_SAFE_RELEASE(_attack);
	CC_SAFE_RELEASE(_heavyAttack);
}

bool ControlLayer::init()
{
	bool ret = false;

	do 
	{
		CC_BREAK_IF(!Layer::init());
		auto visibleSize = Director::getInstance()->getWinSize();

		_joypad = Sprite::create("joypad.png");
		_joypad_bg = Sprite::create("joypad_bg.png");
		this->addChild(_joypad_bg);
		this->addChild(_joypad);
		_inactivityJoypad();

		_attack = Sprite::create("attack.png");
		_heavyAttack = Sprite::create("heavyAttack.png");
		_attack->setPosition(visibleSize.width - 100, 100);
		_heavyAttack->setPosition(visibleSize.width - 200, 100);
		this->addChild(_attack);
		this->addChild(_heavyAttack);

		auto listener = EventListenerTouchAllAtOnce::create();
		listener->onTouchesBegan = CC_CALLBACK_2(ControlLayer::onTouchesBegan, this);
		listener->onTouchesMoved = CC_CALLBACK_2(ControlLayer::onTouchesMoved, this);
		listener->onTouchesEnded = CC_CALLBACK_2(ControlLayer::onTouchesEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		ret = true;
	} while (0);

	return ret;
}

bool ControlLayer::attackContain(Vec2 pos)
{
	auto posSprite = _attack->getPosition();
	auto dis = posSprite.getDistance(pos);
	if (dis <= 24)
		return true;
	return false;
}

bool ControlLayer::heavyAttackContain(Vec2 pos)
{
	auto posSprite = _heavyAttack->getPosition();
	auto dis = posSprite.getDistance(pos);
	if (dis <= 24)
		return true;
	return false;
}


void ControlLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event* e)
{
	Size winSize = Director::getInstance()->getWinSize();
	auto iter = touches.begin();

	while (iter != touches.end())
	{
		Touch *touch = (Touch*)(*iter);
		auto p = touch->getLocation();
		// left，当触控操作的起点小于屏幕宽度的一半，说明触控发生在左屏
		if (p.x <= winSize.width / 2)
		{
			_activityJoypad(p);
		}
		else
		{
			//发生在右屏
			if (attackContain(p))
			{
				_delegator->onAttack();
				_delegator->onHurt();//当前层执行了，至于具体是否受击，交由gamelayer判断
			}
				
			if (heavyAttackContain(p))
			{
				heavyAttackAction(_heavyAttack, 1);
				_delegator->onHurt();
			}
		}
		iter++;
	}
}

void ControlLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event* e)
{
	auto winSize = Director::getInstance()->getWinSize();
	auto iter = touches.begin();
	auto touch = (Touch*)(*iter);
	auto start = touch->getStartLocation();
	

	if (start.x > winSize.width / 2) 
	{
		return;
	}
	
	auto posNow = touch->getLocation();
	float distance = start.getDistance(posNow);
	auto direction = (posNow - start).getNormalized();
	
	_updateJoypad(direction, distance);
	_delegator->onWalk(direction, distance);

}

void ControlLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event* e)
{
	_delegator->onStop();//这里还有改进的地方，如果需要实现技能的连携，需要把idle去掉
	_inactivityJoypad();
}

void ControlLayer::heavyAttackAction(Node* target, int time)
{
	static bool isCDOK = true;//静态变量，防止没CD就使用技能
	if (isCDOK == false)return;
	isCDOK = false;
	
	_delegator->onHeavyAttack();

	auto nodeSprite = (Sprite*)target;
	auto texture = nodeSprite->getTexture();

	//创建一个和原图大小一样的半透明黑色遮罩
	auto _Mask = Sprite::createWithTexture(texture);
	_Mask->setAnchorPoint(Vec2::ZERO);
	_Mask->setColor(Color3B(0, 0, 0));
	_Mask->setOpacity(100);
	
	//为Mask创建一个逆时针进度
	auto prog = ProgressTimer::create(_Mask);
	prog->setAnchorPoint(Vec2::ZERO);
	prog->setType(ProgressTimer::Type::RADIAL);
	prog->setReverseProgress(true);//首先设置成逆时针的进度
	nodeSprite->addChild(prog);

	CallFunc* actionCallback = CallFunc::create([prog, nodeSprite]() {nodeSprite->removeChild(prog, true);isCDOK = true;});//移除遮罩
	auto _heavyAttackAction = Sequence::create(ProgressFromTo::create(time, 0, 100)->reverse(), actionCallback, NULL);//再设置成从100%到0，mask就会消失了
	
	//运行
	prog->runAction(_heavyAttackAction);
}

void ControlLayer::_activityJoypad(Vec2 position)
{
	_joypad->setPosition(position);
	_joypad_bg->setPosition(position);
	_joypad->setVisible(true);
	_joypad_bg->setVisible(true);
}

void ControlLayer::_inactivityJoypad()
{
	_joypad->setPosition(_joypad_bg->getPosition());
	_joypad->setVisible(false);
	_joypad_bg->setVisible(false);
}

void ControlLayer::_updateJoypad(Vec2 direction, float distance)
{
	auto start = _joypad_bg->getPosition();

	if (distance < 74)
	{
		// 如果移动未超出“摇杆基座”，“摇杆”在“基座”做偏移
		_joypad->setPosition(start + (direction * distance));
	}
	else if (distance < 128)
	{
		// 如果移动超出“摇杆基座”，“摇杆”边缘始终在“基座”边缘做偏移
		_joypad->setPosition(start + (direction * 32));	
	}
	else
	{
		// 如果移动超出“摇杆基座”，“摇杆”圆心始终在“基座”边缘做偏移
		_joypad->setPosition(start + (direction * 64));
	}
}
