#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CMovingBar: public CGameObject
{
public:
	CMovingBar();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};