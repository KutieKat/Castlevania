#pragma once
#include "Enemy.h"
#include "../../../Utilities/Constants.h"

class CSimon;

class CGhost : public CEnemy
{
public:
	CGhost(CSimon* simon);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void TakeDamage(int damages = ENEMY_DEFAULT_DAMAGES);

protected:
	CSimon* simon;
};