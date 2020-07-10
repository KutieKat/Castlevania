#pragma once
#include "Enemy.h"
#include "../../../Utilities/Constants.h"

class CSimon;

class CGhoul : public CEnemy
{
public:
	CGhoul(CSimon* simon);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void TakeDamage(int damages);

	int GetDamages();

protected:
	CSimon* simon;
};