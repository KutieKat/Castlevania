#pragma once
#include "../../GameObject.h"
#include "../../../Utilities/Constants.h"

class CSpearKnight : public CGameObject
{
public:
	CSpearKnight();

	void SetState(int state);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void TakeDamage(int volumes = 1);

protected:
	int remainingHits;
	DWORD delayTimeout;
};