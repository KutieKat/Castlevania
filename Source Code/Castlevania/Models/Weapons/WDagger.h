#pragma once
#include "../GameObject.h"

class WDagger : public CGameObject
{
public:
	WDagger();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Render();
};

