#pragma once
#include "Weapon.h"

class WDagger : public CWeapon
{
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Render();
};

