#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CBigCandle : public CGameObject
{
public:
	CBigCandle();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Disappear();
};