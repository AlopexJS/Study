#include "SpriteAction.h"

SpriteAction::SpriteAction()
{

}

void SpriteAction::bloodDown(int bd)
{

}

void SpriteAction::setRestrPosition(Vec2 pos)
{
	if (pos.x >= 940)
	{

		if (pos.y >= 250)
		{
			this->setPosition(940, 250);
			return;
		}
		if (pos.y <= 88)
		{
			this->setPosition(940, 88);
			return;
		}
		this->setPosition(940, pos.y);
		return;
	}
	if (pos.x <= -20)
	{

		if (pos.y >= 250)
		{
			this->setPosition(-20, 250);
			return;
		}
		if (pos.y <= 88)
		{
			this->setPosition(-20, 88);
			return;
		}
		this->setPosition(-20, pos.y);
		return;
	}
	if (pos.y >= 250)
	{
		if (pos.x <= -20)
		{
			this->setPosition(-20, 250);
			return;
		}
		if (pos.x >= 940)
		{
			this->setPosition(940, 250);
			return;
		}
		this->setPosition(pos.x, 250);
		return;
	}
	if (pos.y <= 88)
	{
		if (pos.x <= -20)
		{
			this->setPosition(-20, 88);
			return;
		}
		if (pos.x >= 940)
		{
			this->setPosition(940, 88);
			return;
		}
		this->setPosition(pos.x, 88);
		return;
	}
	this->setPosition(pos);
}

SpriteAction::~SpriteAction()
{
	CC_SAFE_RELEASE(m_pActionIdle);
	CC_SAFE_RELEASE(m_pActionWalk);
	CC_SAFE_RELEASE(m_pActionAttack);
	CC_SAFE_RELEASE(m_pActionHurt);
	CC_SAFE_RELEASE(m_pActionDeath);
	CC_SAFE_RELEASE(m_pLossBlood);
}

bool SpriteAction::init()
{
	isDeath = false;
	m_pLossBlood = NULL;
	return true;
}

void SpriteAction::idle()
{
}

void SpriteAction::walk(Vec2 direction)
{
}

void SpriteAction::attack()
{
}

void SpriteAction::hurt(int bd)
{
}

void SpriteAction::death()
{
	
}

void SpriteAction::cleanUp()
{
}

void SpriteAction::onExit()
{
	Node::onExit();
}

//void SpriteAction::update(float delta)
//{
//}
