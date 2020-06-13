#pragma once
#include <string>

#include "GameObject.h"

#include "Characters/Enemies/RedBat.h"
#include "Characters/Enemies/SpearKnight.h"
#include "Characters/Enemies/Ghost.h"
#include "Characters/Enemies/Fleamen.h"
#include "Characters/Enemies/WhiteSkeleton.h"
#include "Characters/Enemies/Raven.h"
#include "Characters/Bosses/PhantomBat.h"

#include "Items/BigHeart.h"
#include "Items/MoneyBag.h"
#include "Items/MorningStar.h"
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
#include "Misc/CameraLocker.h"
#include "Misc/Mountain.h"
#include "Misc/CreditText.h"
#include "Misc/Castle.h"
using namespace std;

class CObjectFactory
{
public:
	static CGameObject* Construct(string type, float x = 0, float y = 0, CSimon* simon = nullptr);
};

