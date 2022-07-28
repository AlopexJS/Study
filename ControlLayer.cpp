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
		// left�������ز��������С����Ļ��ȵ�һ�룬˵�����ط���������
		if (p.x <= winSize.width / 2)
		{
			_activityJoypad(p);
		}
		else
		{
			//����������
			if (attackContain(p))
			{
				_delegator->onAttack();
				_delegator->onHurt();//��ǰ��ִ���ˣ����ھ����Ƿ��ܻ�������gamelayer�ж�
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
	_delegator->onStop();//���ﻹ�иĽ��ĵط��������Ҫʵ�ּ��ܵ���Я����Ҫ��idleȥ��
	_inactivityJoypad();
}

void ControlLayer::heavyAttackAction(Node* target, int time)
{
	static bool isCDOK = true;//��̬��������ֹûCD��ʹ�ü���
	if (isCDOK == false)return;
	isCDOK = false;
	
	_delegator->onHeavyAttack();

	auto nodeSprite = (Sprite*)target;
	auto texture = nodeSprite->getTexture();

	//����һ����ԭͼ��Сһ���İ�͸����ɫ����
	auto _Mask = Sprite::createWithTexture(texture);
	_Mask->setAnchorPoint(Vec2::ZERO);
	_Mask->setColor(Color3B(0, 0, 0));
	_Mask->setOpacity(100);
	
	//ΪMask����һ����ʱ�����
	auto prog = ProgressTimer::create(_Mask);
	prog->setAnchorPoint(Vec2::ZERO);
	prog->setType(ProgressTimer::Type::RADIAL);
	prog->setReverseProgress(true);//�������ó���ʱ��Ľ���
	nodeSprite->addChild(prog);

	CallFunc* actionCallback = CallFunc::create([prog, nodeSprite]() {nodeSprite->removeChild(prog, true);isCDOK = true;});//�Ƴ�����
	auto _heavyAttackAction = Sequence::create(ProgressFromTo::create(time, 0, 100)->reverse(), actionCallback, NULL);//�����óɴ�100%��0��mask�ͻ���ʧ��
	
	//����
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
		// ����ƶ�δ������ҡ�˻���������ҡ�ˡ��ڡ���������ƫ��
		_joypad->setPosition(start + (direction * distance));
	}
	else if (distance < 128)
	{
		// ����ƶ�������ҡ�˻���������ҡ�ˡ���Եʼ���ڡ���������Ե��ƫ��
		_joypad->setPosition(start + (direction * 32));	
	}
	else
	{
		// ����ƶ�������ҡ�˻���������ҡ�ˡ�Բ��ʼ���ڡ���������Ե��ƫ��
		_joypad->setPosition(start + (direction * 64));
	}
}
