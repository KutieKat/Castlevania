#pragma once
#include "Enemy.h"
#include "../../../Utilities/Constants.h"

class CSimon;

class CRedBat : public CEnemy
{
public:
	CRedBat(CSimon* simon);

	void SetState(int state);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void Render();
	void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	void TakeDamage(int damages);
	void OnPlayerEnterArea();

protected:
	CSimon* simon;

	int flyingCounter;
};