#include "ObjectFactory.h"

CGameObject* CObjectFactory::Construct(string type)
{
	if (type == "background")
	{
		return new CBackground();
	}
	else if (type == "intro_bat")
	{
		return new CIntroBat();
	}
	else if (type == "ground")
	{
		return new CGround();
	}
	else if (type == "big_heart")
	{
		return new CBigHeart();
	}
	else if (type == "morning_star")
	{
		return new CMorningStar();
	}
	else if (type == "dagger")
	{
		return new CDagger();
	}
	else if (type == "big_candle")
	{
		return new CBigCandle();
	}
	else if (type == "money_bag")
	{
		return new CMoneyBag();
	}
	else if (type == "easter_egg")
	{
		return new CEasterEgg();
	}
	else if (type == "door")
	{
		return new CDoor();
	}
	else if (type == "helicopter")
	{
		return new CHelicopter();
	}
	else if (type == "bat")
	{
		return new CBat();
	}
	else if (type == "one_thousand")
	{
		return new COneThousand();
	}
	else if (type == "flash")
	{
		return new CFlash();
	}
	else if (type == "map_pin")
	{
		return new CMapPin();
	}
	else
	{
		return nullptr;
	}
}