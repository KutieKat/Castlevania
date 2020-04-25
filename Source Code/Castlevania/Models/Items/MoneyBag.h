#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CMoneyBag : public CGameObject
{
public:
	CMoneyBag();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void SetState(int state);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Render();
};

