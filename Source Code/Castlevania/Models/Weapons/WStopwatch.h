#pragma once
#include "../GameObject.h"

class WStopwatch : public CGameObject
{
public:
	WStopwatch();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Render();
};

