#pragma once
#include "../GameObject.h"

class WAxe : public CGameObject
{
public:
	WAxe();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Render();
};

