#pragma once
#include <string>

#include "GameObject.h"

#include "Characters/Enemies/RedBat.h"
#include "Characters/Enemies/SpearKnight.h"

#include "Items/BigHeart.h"
#include "Items/MoneyBag.h"
#include "Items/MorningStar.h"
#include "Items/EasterEgg.h"
#include "Items/Dagger.h"
#include "Items/RedMoneyBag.h"
#include "Items/PurpleMoneyBag.h"
#include "Items/WhiteMoneyBag.h"
#include "Items/Crown.h"
#include "Items/SmallHeart.h"
#include "Items/DoubleShot.h"
#include "Items/TripleShot.h"
#include "Items/Boomerang.h"
#include "Items/PorkChop.h"
#include "Items/Rosary.h"
#include "Items/Axe.h"
#include "Items/HolyWater.h"
#include "Items/Stopwatch.h"
#include "Items/MagicCrystal.h"
#include "Items/InvisibilityPotion.h"

#include "Effects/Flash.h"
#include "Effects/OneHundred.h"
#include "Effects/FourHundred.h"
#include "Effects/SevenHundred.h"
#include "Effects/OneThousand.h"
#include "Effects/TwoThousand.h"
#include "Effects/Breaking.h"

#include "Misc/Background.h"
#include "Misc/IntroBat.h"
#include "Misc/Ground.h"
#include "Misc/BigCandle.h"
#include "Misc/Bat.h"
#include "Misc/Helicopter.h"
#include "Misc/MapPin.h"
#include "Misc/Brick.h"
#include "Misc/NextScene.h"
#include "Misc/PreviousScene.h"
#include "Misc/MovingBar.h"
#include "Misc/BigDoubleShot.h"
#include "Misc/BigTripleShot.h"
#include "Misc/SmallCandle.h"
#include "Misc/BreakableBrick.h"
using namespace std;

class CObjectFactory
{
public:
	static CGameObject* Construct(string type, float x = 0, float y = 0);
};

