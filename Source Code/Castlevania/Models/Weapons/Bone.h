#pragma once
#include "../GameObject.h"

class WBone : public CGameObject
{
public:
	WBone();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Render();
};

