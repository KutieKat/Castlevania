#pragma once
#include "../GameObject.h"

class CHolyWaterBottle : public CGameObject
{
public:
	CHolyWaterBottle();

	void SetState(int state);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Render();
};

