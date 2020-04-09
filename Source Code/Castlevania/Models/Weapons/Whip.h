#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"
#include "../Weapons/Weapon.h"

class CWhip : public CWeapon
{
public:
	CWhip();

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Upgrade();
};

