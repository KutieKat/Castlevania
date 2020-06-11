#pragma once
#include "../GameObject.h"

class WFireball : public CGameObject
{
public:
	WFireball();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Render();
	void SetTarget(float x, float y);

protected:
	float angle;
	float targetX;
	float targetY;
};

