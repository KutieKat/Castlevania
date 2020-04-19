#include "ObjectFactory.h"

CGameObject* CObjectFactory::Construct(string type)
{
	if (type == "background")
	{
		return new CBackground();
	}

	if (type == "intro_bat")
	{
		return new CIntroBat();
	}

	if (type == "ground")
	{
		return new CGround();
	}

	if (type == "big_heart")
	{
		return new CBigHeart();
	}

	if (type == "morning_star")
	{
		return new CMorningStar();
	}

	if (type == "dagger")
	{
		return new CDagger();
	}

	if (type == "big_candle")
	{
		return new CBigCandle();
	}

	if (type == "money_bag")
	{
		return new CMoneyBag();
	}

	if (type == "easter_egg")
	{
		return new CEasterEgg();
	}

	if (type == "door_wall")
	{
		return new CDoorWall();
	}

	if (type == "door")
	{
		return new CDoor();
	}

	if (type == "helicopter")
	{
		return new CHelicopter();
	}

	if (type == "bat")
	{
		return new CBat();
	}

	if (type == "one_thousand")
	{
		return new COneThousand();
	}

	if (type == "flash")
	{
		return new CFlash();
	}

	return nullptr;
}
