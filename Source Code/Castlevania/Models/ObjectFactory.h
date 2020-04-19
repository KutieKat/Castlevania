#pragma once
#include <string>

#include "GameObject.h"
#include "Misc/Background.h"
#include "Misc/IntroBat.h"
#include "Misc/Ground.h"
#include "Misc/Door.h"
#include "Misc/DoorWall.h"
#include "Misc/BigCandle.h"
#include "Items/BigHeart.h"
#include "Items/MoneyBag.h"
#include "Items/MorningStar.h"
#include "Items/EasterEgg.h"
#include "Items/Dagger.h"
#include "Misc/Bat.h"
#include "Misc/Helicopter.h"
#include "Effects/Flash.h"
#include "Effects/OneThousand.h"
using namespace std;

class CObjectFactory
{
public:
	static CGameObject* Construct(string type);
};
