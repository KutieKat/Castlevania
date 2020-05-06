#pragma once
#include <string>

#include "GameObject.h"

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
#include "Items/Boomerang.h"

#include "Effects/Flash.h"
#include "Effects/OneHundred.h"
#include "Effects/FourHundred.h"
#include "Effects/SevenHundred.h"
#include "Effects/OneThousand.h"
#include "Effects/TwoThousand.h"

#include "Misc/Background.h"
#include "Misc/IntroBat.h"
#include "Misc/Ground.h"
#include "Misc/Door.h"
#include "Misc/BigCandle.h"
#include "Misc/Bat.h"
#include "Misc/Helicopter.h"
#include "Misc/MapPin.h"
#include "Misc/Brick.h"
#include "Misc/NextScene.h"
#include "Misc/PreviousScene.h"
#include "Misc/MovingBar.h"
#include "Misc/BigDoubleShot.h"
#include "Misc/SmallCandle.h"
using namespace std;

class CObjectFactory
{
public:
	static CGameObject* Construct(string type);
};

