#include "ObjectFactory.h"

CGameObject* CObjectFactory::Construct(string type, float x, float y)
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
	else if (type == "one_hundred")
	{
		return new COneHundred();
	}
	else if (type == "four_hundred")
	{
		return new CFourHundred();
	}
	else if (type == "seven_hundred")
	{
		return new CSevenHundred();
	}
	else if (type == "one_thousand")
	{
		return new COneThousand();
	}
	else if (type == "two_thousand")
	{
		return new CTwoThousand();
	}
	else if (type == "flash")
	{
		return new CFlash();
	}
	else if (type == "map_pin")
	{
		return new CMapPin();
	}
	else if (type == "brick")
	{
		return new CBrick();
	}
	else if (type == "next_scene")
	{
		return new CNextScene();
	}
	else if (type == "previous_scene")
	{
		return new CPreviousScene();
	}
	else if (type == "moving_bar")
	{
		return new CMovingBar();
	}
	else if (type == "red_money_bag")
	{
		return new CRedMoneyBag();
	}
	else if (type == "purple_money_bag")
	{
		return new CPurpleMoneyBag();
	}
	else if (type == "white_money_bag")
	{
		return new CWhiteMoneyBag();
	}
	else if (type == "crown")
	{
		return new CCrown();
	}
	else if (type == "small_heart")
	{
		return new CSmallHeart();
	}
	else if (type == "double_shot")
	{
		return new CDoubleShot();
	}
	else if (type == "big_double_shot")
	{
		return new CBigDoubleShot();
	}
	else if (type == "boomerang")
	{
		return new CBoomerang();
	}
	else if (type == "small_candle")
	{
		return new CSmallCandle();
	}
	else if (type == "breaking")
	{
		return new CBreaking(x, y);
	}
	else if (type == "red_bat")
	{
		return new CRedBat();
	}
	else if (type == "spear_knight")
	{
		return new CSpearKnight();
	}
	else
	{
		return nullptr;
	}
}