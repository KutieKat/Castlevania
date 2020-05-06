#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CWhiteMoneyBag : public CGameObject
{
public:
	CWhiteMoneyBag();

	virtual void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};